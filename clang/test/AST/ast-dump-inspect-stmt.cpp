// RUN: %clang_cc1 -fsyntax-only -fpattern-matching -ast-dump %s | FileCheck %s

void TestInspect() {
  inspect(3) {
    __:;
  }
  // CHECK: InspectStmt 0x{{[^ ]*}} <line:[[@LINE-3]]:3, line:[[@LINE-1]]:3>
  // CHECK-NEXT: IntegerLiteral 0x{{[^ ]*}} <line:[[@LINE-4]]:11> 'int' 3
  // CHECK-NEXT: CompoundStmt 0x{{[^ ]*}} <col:14, line:[[@LINE-3]]:3>
  // CHECK-NEXT: WildcardPatternStmt 0x{{[^ ]*}} <line:[[@LINE-5]]:5, col:8>
  // CHECK-NEXT: NullStmt
}

