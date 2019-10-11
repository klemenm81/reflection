#include <Windows.h>

#include "IReflectable.h"
#include "IField.h"
#include "IMethod.h"
#include "IClass.h"
#include "Adaptor.h"

IReflectable& CreateInstance(const char* name) {
	HMODULE hModule = GetModuleHandle(NULL);
	IReflectable& (*AbstractFactory)(const char*) =
		(IReflectable & (*)(const char*))GetProcAddress(hModule, "AbstractFactory");
	return AbstractFactory("Test");
}

int main() {
	IReflectable &test = CreateInstance("Test");

	IField& field1 = test.GetClass().GetField("a");
	IField& field2 = test.GetClass().GetField("myString");
	IField& field3 = test.GetClass().GetField("ptrString");
	IMethod& method1 = test.GetClass().GetMethod("Foo1");
	IMethod& method2 = test.GetClass().GetMethod("Foo2");

	Adaptor adaptor1 = field1.GetValue(test);
	Adaptor adaptor2 = field2.GetValue(test);
	Adaptor adaptor3 = field3.GetValue(test);

	int see1 = adaptor1.Get<int>();
	std::string see2 = adaptor2.Get<std::string>();
	const wchar_t* see3 = adaptor3.Get<const wchar_t*>();

	std::wstring str = L"PI = ";
	std::vector<IAdaptor*> args1;
	args1.push_back(new CAdaptor<std::wstring&>(str));
	args1.push_back(new CAdaptor<float>(3.14f));

	std::vector<IAdaptor*> args2;
	args2.push_back(new CAdaptor<const wchar_t*>(L"Hello Reflected"));

	Adaptor retVal1 = method1.Invoke(test, args1);
	Adaptor retVal2 = method2.Invoke(test, args2);
	printf("Main(): Return from Foo2 = %d\n", retVal2.Get<int>());

	adaptor1.Set(13);
	field1.SetValue(test, adaptor1);

	wprintf(L"Main(): str = %s\n", str.c_str());
}