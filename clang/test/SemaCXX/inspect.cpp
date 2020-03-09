// RUN: %clang_cc1 -fsyntax-only -verify -fpattern-matching %s

// expected-no-diagnostics

void a() {
  inspect(42) {
    __:;
  }
}
