// RUN: %clang_cc1 -triple i386-unknown-unknown -fpattern-matching -emit-llvm %s -o - | FileCheck %s

// CHECK-LABEL: _Z7test_idi(
int test_id(int n) {
  int x = n + 1;       // CHECK: %inspect.result{{.*}}alloca
  int w = inspect(x) { // CHECK: br label %pat.id
    // CHECK: pat.id:
    // CHECK: store{{.*}}%inspect.result
    y => y++; // CHECK: br label %inspect.epilogue
  };
  // CHECK: load{{.*}}%inspect.result
  return x;
}
