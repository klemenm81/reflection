#include "test.h"

Test::Test() : a(5), myString("Reflection"), ptrString(L"Hello") {
}

void Test::Foo(std::wstring& str, float val) {
	str += std::to_wstring(val);
	wprintf(L"Foo(): str = %s\n", str.c_str());
}

int Test::GetA() {
	return a;
}



