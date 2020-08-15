#include <iostream>

/* example 1 */

#include <iostream>
#include <type_traits>
#include <typeinfo>
#include <tuple>

class MyClass {};

void example1()
{
	std::cout << std::is_floating_point<MyClass>::value << '\n';
	std::cout << std::is_floating_point<float>::value << '\n';
	std::cout << std::is_floating_point<int>::value << '\n';
}

/*  example 2 */

template<typename T>
auto fooOrBar(T const& t) -> decltype(t.foo()) {
	return t.foo();
}

template<typename T>
auto fooOrBar(T const& t) -> decltype(t.bar()) {
	return t.bar();
}

void example2() {
	struct HasFoo { void foo() const {} } f;
	struct HasBar { void bar() const {} } b;
	struct HasBoth : HasFoo, HasBar {} fb;

	fooOrBar(f);
	fooOrBar(b);
	//fooOrBar(fb); // error
}

/* example 3 */

template<typename, typename = void>
constexpr bool hasFoo = false;

template<typename T>
constexpr bool hasFoo<T, std::void_t<decltype(std::declval<T>().foo())>> = true;

template <typename T> void callFooIfItExists(T t) {
	if constexpr (hasFoo<T>) {
		std::cout << "T contains foo(), calling foo()" << std::endl;
		t.foo();
	}
	else {
		std::cout << "T does not contain foo()" << std::endl;
	}
}

void example3() {
	struct HasFoo { void foo() const {} } f;
	struct HasBar { void bar() const {} } b;
	struct HasBoth : HasFoo, HasBar {} fb;

	callFooIfItExists(f);
	callFooIfItExists(b);
	callFooIfItExists(fb);
}


/* example 4 */

template <typename Class, typename Return, typename... Args>
struct MyMethodTraits;

template <typename Class, typename Return, typename... Args>
struct MyMethodTraits<Class, Return(Class::*)(Args...)> {
	using ReflectedClass = Class;
	using ReflectedReturnType = Return;
	using ReflectedArgs = std::tuple<Args...>;
	static const bool isConst = false;
};

template <typename Class, typename Return, typename... Args>
struct MyMethodTraits<Class, Return(Class::*)(Args...) const> {
	using ReflectedClass = Class;
	using ReflectedReturnType = Return;
	using ReflectedArgs = std::tuple<Args...>;
	static const bool isConst = true;
};


class Foo {
public:
	int Bar(int arg1, float arg2) const {
		return 0;
	}
};

template <typename... Args>
void printTypesHelper(std::tuple<Args...>) {
	int dummy[] = { 
		(std::cout << typeid(Args).name() << std::endl, 0)... 
	};
}

void example4() {
	Foo foo;

	MyMethodTraits<
		Foo, 
		decltype(&Foo::Bar)
	>::ReflectedReturnType retVal = foo.Bar(1, 12.5);  

	std::cout <<
		"Method Foo::Bar() " <<
		(MyMethodTraits<Foo, decltype(&Foo::Bar)>::isConst ?
			"is a const method" :
			"is not a const method") <<
		std::endl;

	printTypesHelper(
		MyMethodTraits<Foo, decltype(&Foo::Bar)>::ReflectedArgs{}
	);
}


/*  example 5 */

template<typename R, typename... Args>
auto print_default_result(R(*)(Args...)) -> void {
	std::cout << R{} << '\n';
}

auto ret_double() -> double {
	return 0.1;
}

auto example5() -> void {
	print_default_result(ret_double);
}

/* example 6 */
#include <vector>

template <typename T>
struct MyTypeTrait {
	static void what() {
		std::cout << "T is a value" << std::endl;
	}
};
template <typename T>
struct MyTypeTrait<T*> {
	static void what() {
		std::cout << "T is a pointer" << std::endl;
	}
};
template <typename T>
struct MyTypeTrait<T&&> {
	static void what() {
		std::cout << "T is a rvalue reference" << std::endl;
	}
}; 
template <typename T>
struct MyTypeTrait<std::vector<T>> {
	static void what() {
		std::cout << "T is a vector of values" << std::endl;
	}
};

void example6() {
	MyTypeTrait<int>::what();
	MyTypeTrait<double &&>::what();
	MyTypeTrait<float *>::what();
	MyTypeTrait<std::vector<bool>>::what();
}

/* example 7 */

template<typename Type>
constexpr Type square(Type type) {
	return(type * type);
}

void example7() {
	std::cout << square(5) << std::endl;
	std::cout << square(3.14f) << std::endl;
}


/* example 8 */

class A {
};
class B : public A {
};
void example8(A& ref) {
	if (typeid(ref) == typeid(A)) {
		std::cout << "ref is instance of A" << std::endl;
	}
	else if (typeid(ref) == typeid(B)) {
		std::cout << "ref is instance of B" << std::endl;
	}
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
}