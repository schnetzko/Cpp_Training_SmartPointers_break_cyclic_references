//============================================================================
// Name        : Cpp_Training_SmartPointers_break_cyclic_references.cpp
// Author      : Nico
// Version     :
// Copyright   : Your copyright notice
// Description : Testing SmartPointers in C++, Ansi-style
//============================================================================

#include <iostream>
#include <memory>

using namespace std;

struct B;

struct A {
	A() {cout << "A::A ctor called" << endl;};
	~A() {cout << "A::~A dtor called" << endl;};
	shared_ptr<B> b;
};

struct B {
	B() {cout << "B::B ctor called" << endl;};
	~B() {cout << "B::~B dtor called" << endl;};
	shared_ptr<A> a;
};

struct B_fixed;

struct A_fixed {
	A_fixed() {cout << "A_fixed::A_fixed ctor called" << endl;};
	~A_fixed() {cout << "A_fixed::~A_fixed dtor called" << endl;};
	weak_ptr<B_fixed> b; // the fix
};

struct B_fixed{
	B_fixed() {cout << "B_fixed::B_fixed ctor called" << endl;};
	~B_fixed() {cout << "B_fixed::~B_fixed dtor called" << endl;};
	weak_ptr<A_fixed> a; // the fix
};

void test1_cyclic_references() {
	cout << ":: test1_cyclic_references ::" << endl;
	shared_ptr<A> a = make_shared<A>();
	shared_ptr<B> b = make_shared<B>();
	a->b = b;
	b->a = a;
	cout << "2 shared references to A-obj and to B-obj" << endl;
	cout << "\t1. a => A-obj" << endl;
	cout << "\t2. B-obj.a => A-obj (equals to b->a => A-obj)" << endl;
	cout << "\t1. b => B-obj" << endl;
	cout << "\t2. A-obj.b => B-obj (equals to a->b => B-obj)" << endl;
	cout << "end of scope, but no dtor will be called, because" << endl;
	cout << "there is still 1 shared reference to each obj: " << endl;
	cout << "\t1. A-obj.b => obj-B" << endl;
	cout << "\t1. B-obj.a => obj-A" << endl;
	cout << "but there is no external reference to A-obj and B-obj anymore which leads to a memory leak" << endl;
}

void test2_cyclic_references_fixed() {
	cout << ":: test2_cyclic_references ::" << endl;
	shared_ptr<A_fixed> a = make_shared<A_fixed>();
	shared_ptr<B_fixed> b = make_shared<B_fixed>();
	a->b = b;
	b->a = a;
	cout << "2 shared references to A-obj and to B-obj" << endl;
	cout << "but references between these objects are only temporary shared, because these are originally defined as weak references" << endl;
	cout << "\t1. a => A-obj" << endl;
	cout << "\t2. B-obj.a => A-obj (temporary shared reference, because it's weak reference)" << endl;
	cout << "\t1. b => B-obj" << endl;
	cout << "\t2. A-obj.b => B-obj (temporary shared reference, because it's weak reference)" << endl;
	cout << "end of scope and each dtor will be called, because there are only weak references left" << endl;
}

int main() {
	test1_cyclic_references();
	cout << endl;
	test2_cyclic_references_fixed();
	return 0;
}
