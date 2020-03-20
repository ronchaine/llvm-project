// RUN: %clang_cc1 -triple i386-unknown-unknown -fpattern-matching -emit-llvm %s -o - | FileCheck %s

// CHECK-LABEL: _Z13test_wildcardi
int test_wildcard(int n) {
  int x = n+1;
  inspect(n) { // CHECK: br label %pat.wildcard
    // CHECK: pat.wildcard
    __ : x++; // CHECK: br label %inspect.epilogue
  }
  return x;
}

int test_wildcard2(int n) {
  inspect(n) { // CHECK: br label %pat.wildcard
    // CHECK: pat.wildcard
    __ : {
      n++;
      inspect(n) { // CHECK: br label %pat.wildcard1
        // CHECK: inspect.epilogue
        __ : n++; // CHECK: br label %inspect.epilogue3
      }
    }
  }
  // CHECK: inspect.epilogue3
  return n;
}
