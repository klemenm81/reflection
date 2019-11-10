#include <iostream>

/*  example 1 */

template<typename T> // foo version
auto foo_or_bar(T const& t) -> decltype(t.foo()) {
	return t.foo();
}

template<typename T> // bar version
auto foo_or_bar(T const& t) -> decltype(t.bar()) {
	return t.bar();
}

void example1() {
	struct has_foo { void foo() const {} } f;
	struct has_bar { void bar() const {} } b;
	//struct has_both : has_foo, has_bar {} fb;

	foo_or_bar(f);  // calls foo version
	foo_or_bar(b);  // calls bar version
	//foo_or_bar(fb); // error, ambiguous call
}

/* example 2 */

template<typename, typename = void>
constexpr bool has_foo = false;

template<typename T>
constexpr bool has_foo<T, std::void_t<decltype(std::declval<T>().foo())>> = true;

template <typename T> void someGenericMethod(T t) {
	if constexpr (has_foo<T>) {
		printf("Calling foo(), because T contains foo()\n");
		t.foo();
	}
	else {
		printf("Not calling foo(), because T does not contain foo()\n");
	}
}

void example2() {
	struct has_foo { void foo() const {} } f;
	struct has_bar { void bar() const {} } b;
	//struct has_both : has_foo, has_bar {} fb;

	someGenericMethod(f);
	someGenericMethod(b);
}


/* example 3 */

template <typename Class, typename Return, typename... Args>
struct MethodTraits;

template <typename Class, typename Return, typename... Args>
struct MethodTraits<Class, Return(Class::*)(Args...)> {
	using ReflectedClass = Class;
	using ReflectedReturnType = Return;
	using ReflectedArgs = std::tuple<Args...>;
	bool isConst = false;
};

template <typename Class, typename Return, typename... Args>
struct MethodTraits<Class, Return(Class::*)(Args...) const> {
	using ReflectedClass = Class;
	using ReflectedReturnType = Return;
	using ReflectedArgs = std::tuple<Args...>;
	bool isConst = true;
};


class Foo {
public:
	int Bar(int arg1, float arg2) const {
		return 0;
	}
};

void example3() {
	const Foo foo;

	// this deduction has a similar effect as 'auto', just with a more verbose syntax
	MethodTraits<Foo, decltype(&Foo::Bar)>::ReflectedReturnType retVal = foo.Bar(1, 12.5);  // 

	// we reflect on a method pointer type to determine whether the method is const or not
	printf("Method Foo::Bar() %s a const method", MethodTraits<Foo, decltype(&Foo::Bar)>().isConst ? "is" : "is not");
}


/*  example 4 */

template<typename R, typename... Args>
auto print_default_result(R(*)(Args...)) -> void {
	std::cout << R{} << '\n';
}

auto ret_double() -> double {
	return 0.1;
}

auto example4() -> void {
	print_default_result(ret_double);
}

/* main */

int main(void) {
	example1();
	example2();
	example3();
	example4();
}