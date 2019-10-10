#include <Windows.h>

#include "IReflectable.h"
#include "IField.h"
#include "IMethod.h"
#include "IClass.h"
#include "Adaptor.h"

int main() {
	HMODULE hModule = GetModuleHandle(NULL);
	IReflectable& (*AbstractFactory)(const char* ) = 
		(IReflectable & (*)(const char*))GetProcAddress(hModule, "AbstractFactory");

	IReflectable &test = AbstractFactory("Test");
	IField& field1 = test.GetClass().GetField("a");
	IField& field2 = test.GetClass().GetField("myString");
	IField& field3 = test.GetClass().GetField("ptrString");
	IMethod& method = test.GetClass().GetMethod("Foo");

	Adaptor adaptor1 = field1.GetValue(test);
	Adaptor adaptor2 = field2.GetValue(test);
	Adaptor adaptor3 = field3.GetValue(test);

	int see1 = adaptor1.Get<int>();
	std::string see2 = adaptor2.Get<std::string>();
	const wchar_t* see3 = adaptor3.Get<const wchar_t*>();

	std::wstring str = L"PI = ";
	std::vector<IAdaptor*> args;
	args.push_back(new CAdaptor<std::wstring&>(str));
	args.push_back(new CAdaptor<float>(3.14f));

	Adaptor retValAdaptor = method.Invoke(test, args);

	adaptor1.Set(13);
	field1.SetValue(test, adaptor1);

	wprintf(L"Main(): str = %s\n", str.c_str());
}