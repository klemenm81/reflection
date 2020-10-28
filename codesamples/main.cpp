#include <iostream>

/* example 1 */

#include <iostream>
#include <type_traits>
#include <typeinfo>
#include <tuple>

using namespace std;

class MyClass {};
class MyClassEx : public MyClass {};

void example1()
{
	cout << is_floating_point<float>::value << '\n';
	cout << is_integral<int>::value << '\n';
	cout << is_object<MyClass>::value << '\n';
	cout << is_polymorphic<MyClass>::value << '\n';
	cout << is_base_of<MyClass, MyClassEx>::value << '\n';
	cout << is_copy_constructible<MyClass>::value << '\n';
}

/*  example 2 */


auto example2() {
	int a = 5;
	decltype(a) b = 10 + a;
	auto c = 15 + b;
	return c;
}


/*  example 3 */

template<typename T>
auto fooOrBar(T const& t) -> decltype(t.foo()) {
	return t.foo();
}

template<typename T>
auto fooOrBar(T const& t) -> decltype(t.bar()) {
	return t.bar();
}

void example3() {
	struct HasFoo { void foo() const {} } f;
	struct HasBar { void bar() const {} } b;
	struct HasBoth : HasFoo, HasBar {} fb;

	fooOrBar(f);
	fooOrBar(b);
	//fooOrBar(fb); // error
}

/* example 4 */

template<typename, typename = void>
constexpr bool hasFoo = false;

template<typename T>
constexpr bool hasFoo<T, void_t<decltype(declval<T>().foo())>> = true;

template <typename T> void callFooIfItExists(T t) {
	if constexpr (hasFoo<T>) {
		cout << "T contains foo(), calling foo()" << endl;
		t.foo();
	}
	else {
		cout << "T does not contain foo()" << endl;
	}
}

void example4() {
	struct HasFoo { void foo() const {} } f;
	struct HasBar { void bar() const {} } b;
	struct HasBoth : HasFoo, HasBar {} fb;

	callFooIfItExists(f);
	callFooIfItExists(b);
	callFooIfItExists(fb);
}


/* example 5 */

template <typename Class, typename Return, typename... Args>
struct MyMethodTraits;

template <typename Class, typename Return, typename... Args>
struct MyMethodTraits<Class, Return(Class::*)(Args...)> {
	using ReflectedClass = Class;
	using ReflectedReturnType = Return;
	using ReflectedArgs = tuple<Args...>;
	static const bool isConst = false;
};

template <typename Class, typename Return, typename... Args>
struct MyMethodTraits<Class, Return(Class::*)(Args...) const> {
	using ReflectedClass = Class;
	using ReflectedReturnType = Return;
	using ReflectedArgs = tuple<Args...>;
	static const bool isConst = true;
};


class Foo {
public:
	int Bar(int arg1, float arg2) const {
		return 0;
	}
};

template <typename... Args>
void printTypesHelper(tuple<Args...>) {
	int dummy[] = { 
		(cout << typeid(Args).name() << endl, 0)... 
	};
}

void example5() {
	Foo foo;

	cout <<
		"Method Foo::Bar() " <<
		(MyMethodTraits<Foo, decltype(&Foo::Bar)>::isConst ?
			"is a const method" :
			"is not a const method") <<
		endl;

	cout <<
		typeid(MyMethodTraits<Foo, decltype(
			&Foo::Bar
			)>::ReflectedReturnType).name() <<
		endl;

	printTypesHelper(
		MyMethodTraits<Foo, decltype(&Foo::Bar)>::ReflectedArgs{}
	);
}


/*  example 6 */

template<typename R, typename... Args>
auto print_default_result(R(*)(Args...)) -> void {
	cout << R{} << '\n';
}

auto ret_double() -> double {
	return 0.1;
}

auto example6() -> void {
	print_default_result(ret_double);
}

/* example 7 */
#include <vector>

template <typename T>
struct MyTypeTrait {
	static void what() {
		cout << "T is a value" << endl;
	}
};
template <typename T>
struct MyTypeTrait<T*> {
	static void what() {
		cout << "T is a pointer" << endl;
	}
};
template <typename T>
struct MyTypeTrait<T&&> {
	static void what() {
		cout << "T is a rvalue reference" << endl;
	}
}; 
template <typename T>
struct MyTypeTrait<vector<T>> {
	static void what() {
		cout << "T is a vector of values" << endl;
	}
};

void example7() {
	MyTypeTrait<int>::what();
	MyTypeTrait<double &&>::what();
	MyTypeTrait<float *>::what();
	MyTypeTrait<vector<bool>>::what();
}

/* example 8 */

template<typename Type>
constexpr Type square(Type type) {
	return(type * type);
}

void example8() {
	cout << square(5) << endl;
	cout << square(3.14f) << endl;
}


/* example 9 */

class A {
};
class B : public A {
};
void example9(A& ref) {
	if (typeid(ref) == typeid(A)) {
		cout << "ref is instance of A" << endl;
	}
	else if (typeid(ref) == typeid(B)) {
		cout << "ref is instance of B" << endl;
	}
}

/* example 10 */
template <typename T>
void overload(T *t) {
}

template <typename T>
void overload(T &t) {
}

//void overload(int&* t) {} // error

struct AA {
	int a;
};
struct BB : public AA {
	int b;
};
void example10() {
	BB b1;
	b1.a = 1;
	b1.b = 1;
	BB b2;
	b2.a = 2;
	b2.b = 2;
	AA& a = b2;
	a = b1;
}

/* main */

int main(void) {
	example1();
	example2();
	example3();
	example4();
	example5();
	example6();
	example7();
	example8();
	//example9();
	example10();
}