#include <Windows.h>

#include "IReflectable.h"
#include "Field.h"
#include "Method.h"
#include "IClass.h"

IReflectable& CreateInstance(const char* name) {
	static HMODULE hModule = GetModuleHandle(NULL);
	IReflectable& (*AbstractFactory)(const char*) =
		(IReflectable & (*)(const char*))GetProcAddress(hModule, "AbstractFactory");
	return AbstractFactory("Test");
}


int main() {
	IReflectable &test = CreateInstance("Test");

	Field field1 = test.GetClass().GetField("a");
	Field field2 = test.GetClass().GetField("myString");
	Field field3 = test.GetClass().GetField("ptrString");
	Method method1 = test.GetClass().GetMethod("Foo1");
	Method method2 = test.GetClass().GetMethod("Foo2");
	Method constMethod = test.GetClass().GetMethod("FooConst");

	int see1 = field1.Get<int>(test);
	std::string see2 = field2.Get<std::string>(test);
	const wchar_t* see3 = field3.Get<const wchar_t*>(test);

	std::wstring str = L"PI = ";

	method1.PushArg<std::wstring&>(str);
	method1.PushArg<float>(3.14f);
	method2.PushArg<const wchar_t*>(L"Hello Reflected");
	
	method1.Invoke(test);
	method2.Invoke(test);
	wprintf(L"Main(): Return from Foo2 = %d\n", method2.GetRetVal<int>());

	constMethod.Invoke(test);
	printf("Main(): Return from FooConst = %s\n", constMethod.GetRetVal<std::string>().c_str());

	field1.Set(test, 13);
	wprintf(L"Main(): str = %s\n", str.c_str());
}