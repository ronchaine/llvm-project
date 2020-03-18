// RUN: %clang_cc1 -triple i386-unknown-unknown -fpattern-matching -emit-llvm %s -o - | FileCheck %s

// CHECK-LABEL: _Z13test_wildcardi
int test_wildcard(int n) {
  int x = n+1;
  inspect(n) { // CHECK: br label %pattern.test0
    // CHECK: pattern.test0
    __ : x++; // CHECK: br label %inspect.end
  }
  return x;
}
