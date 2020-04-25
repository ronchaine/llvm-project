// RUN: %clang_cc1 -fsyntax-only -verify -fpattern-matching %s

void NoParen() {
  inspect 42 { // expected-error {{expected '(' after 'inspect'}}
    __:;
  }
}

void NoParenConstExpr() {
  inspect constexpr 42 { // expected-error {{expected '(' after 'constexpr'}}
    __:;
  }
}

void BasicWorkingInspects() {
  inspect(42) {}

  inspect constexpr(42) {}
}
