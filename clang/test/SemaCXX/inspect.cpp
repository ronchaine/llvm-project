// RUN: %clang_cc1 -fsyntax-only -verify -fpattern-matching %s

void a() {
  inspect(42) {
    __ => {};
  };
  inspect(b) { // expected-error {{use of undeclared identifier 'b'}}
    __ if (true) => {};
  };
}
