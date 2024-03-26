//===--- StmtCXX.cpp - Classes for representing C++ statements ------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the subclesses of Stmt class declared in StmtCXX.h
//
//===----------------------------------------------------------------------===//

#include "clang/AST/StmtCXX.h"

#include "clang/AST/ASTContext.h"

using namespace clang;

QualType CXXCatchStmt::getCaughtType() const {
  if (ExceptionDecl)
    return ExceptionDecl->getType();
  return QualType();
}

CXXTryStmt *CXXTryStmt::Create(const ASTContext &C, SourceLocation tryLoc,
                               CompoundStmt *tryBlock,
                               ArrayRef<Stmt *> handlers) {
  const size_t Size = totalSizeToAlloc<Stmt *>(handlers.size() + 1);
  void *Mem = C.Allocate(Size, alignof(CXXTryStmt));
  return new (Mem) CXXTryStmt(tryLoc, tryBlock, handlers);
}

CXXTryStmt *CXXTryStmt::Create(const ASTContext &C, EmptyShell Empty,
                               unsigned numHandlers) {
  const size_t Size = totalSizeToAlloc<Stmt *>(numHandlers + 1);
  void *Mem = C.Allocate(Size, alignof(CXXTryStmt));
  return new (Mem) CXXTryStmt(Empty, numHandlers);
}

CXXTryStmt::CXXTryStmt(SourceLocation tryLoc, CompoundStmt *tryBlock,
                       ArrayRef<Stmt *> handlers)
    : Stmt(CXXTryStmtClass), TryLoc(tryLoc), NumHandlers(handlers.size()) {
  Stmt **Stmts = getStmts();
  Stmts[0] = tryBlock;
  std::copy(handlers.begin(), handlers.end(), Stmts + 1);
}

CXXForRangeStmt::CXXForRangeStmt(Stmt *Init, DeclStmt *Range,
                                 DeclStmt *BeginStmt, DeclStmt *EndStmt,
                                 Expr *Cond, Expr *Inc, DeclStmt *LoopVar,
                                 Stmt *Body, SourceLocation FL,
                                 SourceLocation CAL, SourceLocation CL,
                                 SourceLocation RPL)
    : Stmt(CXXForRangeStmtClass), ForLoc(FL), CoawaitLoc(CAL), ColonLoc(CL),
      RParenLoc(RPL) {
  SubExprs[INIT] = Init;
  SubExprs[RANGE] = Range;
  SubExprs[BEGINSTMT] = BeginStmt;
  SubExprs[ENDSTMT] = EndStmt;
  SubExprs[COND] = Cond;
  SubExprs[INC] = Inc;
  SubExprs[LOOPVAR] = LoopVar;
  SubExprs[BODY] = Body;
}

Expr *CXXForRangeStmt::getRangeInit() {
  DeclStmt *RangeStmt = getRangeStmt();
  VarDecl *RangeDecl = dyn_cast_or_null<VarDecl>(RangeStmt->getSingleDecl());
  assert(RangeDecl && "for-range should have a single var decl");
  return RangeDecl->getInit();
}

const Expr *CXXForRangeStmt::getRangeInit() const {
  return const_cast<CXXForRangeStmt *>(this)->getRangeInit();
}

VarDecl *CXXForRangeStmt::getLoopVariable() {
  Decl *LV = cast<DeclStmt>(getLoopVarStmt())->getSingleDecl();
  assert(LV && "No loop variable in CXXForRangeStmt");
  return cast<VarDecl>(LV);
}

const VarDecl *CXXForRangeStmt::getLoopVariable() const {
  return const_cast<CXXForRangeStmt *>(this)->getLoopVariable();
}

CoroutineBodyStmt *CoroutineBodyStmt::Create(
    const ASTContext &C, CoroutineBodyStmt::CtorArgs const &Args) {
  std::size_t Size = totalSizeToAlloc<Stmt *>(
      CoroutineBodyStmt::FirstParamMove + Args.ParamMoves.size());

  void *Mem = C.Allocate(Size, alignof(CoroutineBodyStmt));
  return new (Mem) CoroutineBodyStmt(Args);
}

CoroutineBodyStmt *CoroutineBodyStmt::Create(const ASTContext &C, EmptyShell,
                                             unsigned NumParams) {
  std::size_t Size = totalSizeToAlloc<Stmt *>(
      CoroutineBodyStmt::FirstParamMove + NumParams);

  void *Mem = C.Allocate(Size, alignof(CoroutineBodyStmt));
  auto *Result = new (Mem) CoroutineBodyStmt(CtorArgs());
  Result->NumParams = NumParams;
  auto *ParamBegin = Result->getStoredStmts() + SubStmt::FirstParamMove;
  std::uninitialized_fill(ParamBegin, ParamBegin + NumParams,
                          static_cast<Stmt *>(nullptr));
  return Result;
}

CoroutineBodyStmt::CoroutineBodyStmt(CoroutineBodyStmt::CtorArgs const &Args)
    : Stmt(CoroutineBodyStmtClass), NumParams(Args.ParamMoves.size()) {
  Stmt **SubStmts = getStoredStmts();
  SubStmts[CoroutineBodyStmt::Body] = Args.Body;
  SubStmts[CoroutineBodyStmt::Promise] = Args.Promise;
  SubStmts[CoroutineBodyStmt::InitSuspend] = Args.InitialSuspend;
  SubStmts[CoroutineBodyStmt::FinalSuspend] = Args.FinalSuspend;
  SubStmts[CoroutineBodyStmt::OnException] = Args.OnException;
  SubStmts[CoroutineBodyStmt::OnFallthrough] = Args.OnFallthrough;
  SubStmts[CoroutineBodyStmt::Allocate] = Args.Allocate;
  SubStmts[CoroutineBodyStmt::Deallocate] = Args.Deallocate;
  SubStmts[CoroutineBodyStmt::ResultDecl] = Args.ResultDecl;
  SubStmts[CoroutineBodyStmt::ReturnValue] = Args.ReturnValue;
  SubStmts[CoroutineBodyStmt::ReturnStmt] = Args.ReturnStmt;
  SubStmts[CoroutineBodyStmt::ReturnStmtOnAllocFailure] =
      Args.ReturnStmtOnAllocFailure;
  std::copy(Args.ParamMoves.begin(), Args.ParamMoves.end(),
            const_cast<Stmt **>(getParamMoves().data()));
}

InspectStmt::InspectStmt(const ASTContext &Ctx, Stmt *Init, 
                         VarDecl *Var, Expr *Cond)
  : Stmt(InspectStmtClass), FirstPattern(nullptr) {

  bool HasInit = Init != nullptr;
  bool HasVar = Var != nullptr;
  InspectStmtBits.HasInit = HasInit;
  InspectStmtBits.HasVar = HasVar;

  setCond(Cond);
  if (HasInit)
    setInit(Init);
  if (HasVar)
    setConditionVariable(Ctx, Var);

  setInspectLoc(SourceLocation{});
}

InspectStmt::InspectStmt(EmptyShell Empty, bool HasInit, bool HasVar)
  : Stmt(InspectStmtClass, Empty) {

  InspectStmtBits.HasInit = HasInit;
  InspectStmtBits.HasVar = HasVar;
}

InspectStmt *InspectStmt::Create(const ASTContext &Ctx, Stmt *Init,
                                 VarDecl *Var, Expr *Cond) {
  void *Mem = Ctx.Allocate(
    totalSizeToAlloc<Stmt *>(NumMandatoryStmtPtr),
    alignof(InspectStmt));
  return new (Mem) InspectStmt(Ctx, Init, Var, Cond);
}

InspectStmt *InspectStmt::CreateEmpty(const ASTContext &Ctx, bool HasInit, bool HasVar) {
  void *Mem = Ctx.Allocate(
    totalSizeToAlloc<Stmt *>(NumMandatoryStmtPtr),
    alignof(SwitchStmt));
  return new (Mem) InspectStmt(EmptyShell(), HasInit, HasVar);
}

VarDecl *InspectStmt::getConditionVariable() {
  auto *DS = getConditionVariableDeclStmt();
  if (!DS)
    return nullptr;
  return cast<VarDecl>(DS->getSingleDecl());
}

void InspectStmt::setConditionVariable(const ASTContext &Ctx, VarDecl *V) {
  assert(hasVarStorage() &&
    "This inspect statement has no storage for a condition variable!");

  if (!V) {
    getTrailingObjects<Stmt *>()[varOffset()] = nullptr;
    return;
  }

  SourceRange VarRange = V->getSourceRange();
  getTrailingObjects<Stmt *>()[varOffset()] = new (Ctx)
    DeclStmt(DeclGroupRef(V), VarRange.getBegin(), VarRange.getEnd());
}

WildcardPatternStmt *WildcardPatternStmt::Create(const ASTContext &Ctx, SourceLocation wildcardLoc, 
                                                 SourceLocation colonLoc, Expr *patternGuard) {
  bool hasPatternGuard = patternGuard != nullptr;

  void *Mem = Ctx.Allocate(
    totalSizeToAlloc<Stmt *, Expr *>(NumMandatoryStmtPtr, hasPatternGuard),
    alignof(WildcardPatternStmt));
  return new (Mem) WildcardPatternStmt(wildcardLoc, colonLoc, nullptr, patternGuard);
}

WildcardPatternStmt *WildcardPatternStmt::CreateEmpty(const ASTContext &Ctx, bool HasPatternGuard) {
  void *Mem = Ctx.Allocate(
    totalSizeToAlloc<Stmt *, Expr *>(NumMandatoryStmtPtr, HasPatternGuard),
    alignof(WildcardPatternStmt));
  return new (Mem) WildcardPatternStmt(EmptyShell(), HasPatternGuard);
}

IdentifierPatternStmt *IdentifierPatternStmt::Create(const ASTContext &Ctx, SourceLocation caseLoc,
                                                     SourceLocation colonLoc, Expr *patternGuard) {
  bool HasPatternGuard = patternGuard != nullptr;

  void *Mem = Ctx.Allocate(
    totalSizeToAlloc<Stmt *, Expr *>(NumMandatoryStmtPtr, HasPatternGuard),
    alignof(IdentifierPatternStmt));
  return new (Mem) IdentifierPatternStmt(caseLoc, colonLoc, nullptr, nullptr, patternGuard);
}

IdentifierPatternStmt *IdentifierPatternStmt::CreateEmpty(const ASTContext &Ctx, bool HasPatternGuard) {
  void *Mem = Ctx.Allocate(
    totalSizeToAlloc<Stmt *, Expr *>(NumMandatoryStmtPtr, HasPatternGuard),
    alignof(IdentifierPatternStmt));
  return new (Mem) IdentifierPatternStmt(EmptyShell(), HasPatternGuard);
}

ExpressionPatternStmt *ExpressionPatternStmt::Create(const ASTContext &Ctx, SourceLocation caseLoc, 
                                                     SourceLocation colonLoc, Expr *patternGuard) {
  bool hasPatternGuard = patternGuard != nullptr;

  void *Mem = Ctx.Allocate(
    totalSizeToAlloc<Stmt *, Expr *>(NumMandatoryStmtPtr, hasPatternGuard),
    alignof(ExpressionPatternStmt));
  return new (Mem) ExpressionPatternStmt(caseLoc, colonLoc, nullptr, nullptr, patternGuard);
}

ExpressionPatternStmt *ExpressionPatternStmt::CreateEmpty(const ASTContext &Ctx, bool HasPatternGuard) {
  void *Mem = Ctx.Allocate(
    totalSizeToAlloc<Stmt *, Expr *>(NumMandatoryStmtPtr, HasPatternGuard),
    alignof(ExpressionPatternStmt));
  return new (Mem) ExpressionPatternStmt(EmptyShell(), HasPatternGuard);
}
