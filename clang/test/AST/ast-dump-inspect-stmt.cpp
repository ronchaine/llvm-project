// RUN: %clang_cc1 -fsyntax-only -fpattern-matching -ast-dump %s | FileCheck %s

void TestInspect(int a, int b) {
  inspect(3) {
    __ =>;
  }
  // CHECK: InspectStmt 0x{{[^ ]*}} <line:[[@LINE-3]]:3, line:[[@LINE-1]]:3> has_implicit_result_type
  // CHECK-NEXT: IntegerLiteral 0x{{[^ ]*}} <line:[[@LINE-4]]:11> 'int' 3
  // CHECK-NEXT: CompoundStmt 0x{{[^ ]*}} <col:14, line:[[@LINE-3]]:3>
  // CHECK-NEXT: WildcardPatternStmt 0x{{[^ ]*}} <line:[[@LINE-5]]:5, col:10>
  // CHECK-NEXT: NullStmt

  inspect(a) {
    __ if (b>0) =>;
  }
  // CHECK: InspectStmt 0x{{[^ ]*}} <line:[[@LINE-3]]:3, line:[[@LINE-1]]:3> has_implicit_result_type
  // CHECK: WildcardPatternStmt 0x{{[^ ]*}} <line:[[@LINE-3]]:5, col:19> has_guard
  // CHECK-NEXT: NullStmt 0x{{[^ ]*}} <col:19>
  // CHECK-NEXT: BinaryOperator 0x{{[^ ]*}} <col:12, col:14> 'bool' '>'

  inspect(3) -> int {
    __ =>;
  }
  // CHECK: InspectStmt 0x{{[^ ]*}} <line:[[@LINE-3]]:3, line:[[@LINE-1]]:3> has_explicit_result_type
  // CHECK-NEXT: IntegerLiteral 0x{{[^ ]*}} <line:[[@LINE-4]]:11> 'int' 3
  // CHECK-NEXT: CompoundStmt 0x{{[^ ]*}} <col:21, line:[[@LINE-3]]:3>
  // CHECK-NEXT: WildcardPatternStmt 0x{{[^ ]*}} <line:[[@LINE-5]]:5, col:10>
  // CHECK-NEXT: NullStmt

  int x = 3;
  inspect(x) {
    y => y++;
  }
  // CHECK: InspectStmt 0x{{[^ ]*}} <line:[[@LINE-3]]:3, line:[[@LINE-1]]:3> has_implicit_result_type
  // CHECK: IdentifierPatternStmt 0x{{[^ ]*}} <line:[[@LINE-3]]:5, col:11>
  // CHECK-NEXT: UnaryOperator 0x{{[^ ]*}} <col:10, col:11> 'int':'int' postfix '++'
  // CHECK-NEXT: DeclRefExpr 0x{{[^ ]*}} <col:10> 'int':'int' lvalue Var 0x{{[^ ]*}} 'y' 'int &'
  // CHECK-NEXT: DeclStmt 0x{{[^ ]*}} <col:7>
  // CHECK-NEXT: VarDecl 0x{{[^ ]*}} <col:5, line:[[@LINE-8]]:11> line:[[@LINE-7]]:5 used y 'int &' auto cinit
}

