// RUN: %clang_cc1 -fsyntax-only -verify -fpattern-matching %s

void a() {
  inspect 42 { // expected-error {{expected '(' after 'inspect'}}
    __:;
  }
}
