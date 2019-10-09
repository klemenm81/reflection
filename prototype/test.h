#pragma once

#include "Reflectable.h"
#include "Adaptor.h"

#include <vector>

class Test : public Reflectable<Test> {
	friend class CClass<Test>;		// if we want to have access to private and protected members via reflection, we need this friend

private:
	int a;
	std::string myString;
	const wchar_t* ptrString;

public:
	Test();
	void Foo(std::wstring& str, float val);
	int GetA();
};