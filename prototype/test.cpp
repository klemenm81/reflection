#include "test.h"

Test::Test() : a(5), myString("Reflection"), ptrString(L"Hello") {
}

void Test::Foo1(std::wstring& str, float val) {
	str += std::to_wstring(val);
	wprintf(L"Foo(): str = %s\n", str.c_str());
}

int Test::Foo2(const wchar_t* str) {
	wprintf(L"Foo2(): str = %s\n", str);
	return 13;
}

int Test::FooOverloaded(int i) {
	return 0;
}

int Test::FooOverloaded(float i) {
	return 1;
}

int Test::FooOverloaded(std::vector<int> i) const {
	return -1;
}

std::string Test::FooConst() const {
	return "From FooConst()";
}

