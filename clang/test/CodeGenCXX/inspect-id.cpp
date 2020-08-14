// RUN: %clang_cc1 -triple i386-unknown-unknown -fpattern-matching -emit-llvm %s -o - | FileCheck %s

// CHECK-LABEL: _Z13test_wildcardi
int test_id(int n) {
  int x = n+1;
  inspect(x) { // CHECK: br label %pat.id
    // CHECK: pat.id
    y : y++; // CHECK: br label %inspect.epilogue
  }
  return x;
}
