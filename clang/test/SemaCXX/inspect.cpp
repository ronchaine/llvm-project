// RUN: %clang_cc1 -fsyntax-only -verify -fpattern-matching -Wno-unused-value %s

void a() {
  inspect(42) {
    __ => {};
  };
  inspect(b) { // expected-error {{use of undeclared identifier 'b'}}
    __ if (true) => {};
  };
}

void b(int x) {
  inspect(x) -> void {
    __ => 3; // expected-error {{cannot initialize statement expression result of type 'void' with an rvalue of type 'int'}}
  };
}

int c(int x) {
  int w = inspect(x) -> int {
    __ => 2.3; // expected-warning {{implicit conversion from 'double' to 'int' changes value from 2.3 to 2}}
  };
  return w;
}

int bar() { return 4; }

void d(int x) {
  inspect(x) -> void {
    __ => (void)bar();
  };
}

void e(int x) {
  inspect(x) -> void {
    __ => { bar(); };
  };
}

void f(int x) {
  inspect(x) -> int {
    __ => { bar(); }; // expected-error {{resulting expression type 'void' must match trailing result type 'int'}}
  };
}

void g(int x) {
  inspect(x) -> void {
    __ => (void)3;
    __ =>;
    __ => { bar(); };
  };
}

void h(int x) {
  inspect(x) {
    __ =>;
    __ => (void)3;
  };
}
