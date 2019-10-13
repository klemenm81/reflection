#pragma once

#include "Reflectable.h"
#include "Adaptor.h"

#include <vector>

class Test : public Reflectable<Test> {
	REFLECT_ALL_ACCESS;		// enables access to private and protected members via reflection

private:
	int a;
	std::string myString;
	const wchar_t* ptrString;

public:
	Test();
	void Foo1(std::wstring& str, float val);
	int Foo2(const wchar_t* str);
	std::string FooConst() const;
};