// RUN: %clang_cc1 -fsyntax-only -fpattern-matching -ast-dump -Wno-unused-value %s | FileCheck %s

void TestInspect(int a, int b) {
  inspect(3) {
    __ => {};
  };
  // CHECK: InspectExpr 0x{{[^ ]*}} <line:[[@LINE-3]]:3, line:[[@LINE-1]]:3> 'void' has_implicit_result_type
  // CHECK-NEXT: IntegerLiteral 0x{{[^ ]*}} <line:[[@LINE-4]]:11> 'int' 3
  // CHECK-NEXT: CompoundStmt 0x{{[^ ]*}} <col:14, line:[[@LINE-3]]:3>
  // CHECK-NEXT: WildcardPatternStmt 0x{{[^ ]*}} <line:[[@LINE-5]]:5, col:12>
  // CHECK-NEXT: CompoundStmt

  inspect(a) {
    __ if (b>0) => {};
  };
  // CHECK: InspectExpr 0x{{[^ ]*}} <line:[[@LINE-3]]:3, line:[[@LINE-1]]:3> 'void' has_implicit_result_type
  // CHECK: WildcardPatternStmt 0x{{[^ ]*}} <line:[[@LINE-3]]:5, col:21> has_guard
  // CHECK-NEXT: CompoundStmt
  // CHECK-NEXT: BinaryOperator 0x{{[^ ]*}} <col:12, col:14> 'bool' '>'

  inspect(3) -> void {
    __ => {};
  };
  // CHECK: InspectExpr 0x{{[^ ]*}} <line:[[@LINE-3]]:3, line:[[@LINE-1]]:3> 'void' has_explicit_result_type
  // CHECK-NEXT: IntegerLiteral 0x{{[^ ]*}} <line:[[@LINE-4]]:11> 'int' 3
  // CHECK-NEXT: CompoundStmt
  // CHECK-NEXT: WildcardPatternStmt 0x{{[^ ]*}} <line:[[@LINE-5]]:5, col:12>
  // CHECK-NEXT: CompoundStmt

  int x = 3;
  int w = inspect(x) -> int {
    y => y++;
  };
  // CHECK: InspectExpr {{.*}}'int' has_explicit_result_type
  // CHECK: IdentifierPatternStmt 0x{{[^ ]*}} <line:[[@LINE-3]]:5, col:11>
  // CHECK-NEXT: UnaryOperator 0x{{[^ ]*}} <col:10, col:11> 'int':'int' postfix '++'
  // CHECK-NEXT: DeclRefExpr 0x{{[^ ]*}} <col:10> 'int':'int' lvalue Var 0x{{[^ ]*}} 'y' 'int &'
  // CHECK-NEXT: DeclStmt 0x{{[^ ]*}} <col:7>
  // CHECK-NEXT: VarDecl 0x{{.*}} used y 'int &' auto cinit

  int v = inspect(x) -> int {
    7 => 4;
  };
  // CHECK: ExpressionPatternStmt {{.*}} <line:[[@LINE-2]]:5, col:10>
  // CHECK-NEXT: IntegerLiteral {{.*}} <col:10> 'int' 4
  // CHECK-NEXT: BinaryOperator {{.*}} <col:5, line:[[@LINE-5]]:19> 'bool' '=='
  // CHECK-NEXT: IntegerLiteral {{.*}} <line:[[@LINE-5]]:5> 'int' 7
  // CHECK-NEXT: ImplicitCastExpr {{.*}} <line:[[@LINE-7]]:19> 'int' <LValueToRValue>
  // CHECK-NEXT: DeclRefExpr {{.*}} <col:19> 'int' lvalue Var {{.*}} 'x' 'int'

  inspect(x) {
    case 7 =>;
  };
  // CHECK: ExpressionPatternStmt {{.*}} <line:[[@LINE-2]]:10, col:14> has_case

  enum class Color { Red, Green, Blue };
  enum Color2 { Red, Green, Blue };

  inspect (x) {
    (int)Color::Red => {}
    case Green => {}
    8 => {}
    Green if(x>0) => {} // should be parsed as identifier pattern
    __ => {}
  };
  // CHECK: InspectExpr {{.*}} 'void' has_implicit_result_type
  // CHECK: ExpressionPatternStmt
  // CHECK: ExpressionPatternStmt
  // CHECK: ExpressionPatternStmt
  // CHECK: IdentifierPatternStmt
  // CHECK: WildcardPatternStmt

}

