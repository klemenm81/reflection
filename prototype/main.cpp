#ifdef _WIN32
    #include <Windows.h>
#else
    #include <dlfcn.h>
#endif

#include <stdio.h>
#include <errno.h>

#include "IReflectable.h"
#include "Field.h"
#include "Method.h"
#include "IClass.h"
#include "test.h"

IReflectable& CreateInstance(const char* name) {
#ifdef _WIN32
	static HMODULE hModule = GetModuleHandle(NULL);
	IReflectable& (*AbstractFactory)(const char*) =
		(IReflectable & (*)(const char*))GetProcAddress(hModule, "AbstractFactory");
#else
	void *hModule = dlopen(NULL, RTLD_NOW | RTLD_LOCAL);
	if (hModule == NULL) {
		perror("dlopen");
		exit(1);
	}
	IReflectable& (*AbstractFactory)(const char*) =
                (IReflectable & (*)(const char*))dlsym(hModule, "AbstractFactory");
	if (AbstractFactory == NULL) {
		printf("AbstractFactory not found\n");
		exit(1);
	}
#endif
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
	//Method rvalMethod = test.GetClass().GetMethod("void Bar17() &&");

	int see1 = field1.Get<int>(test);
	std::string see2 = field2.Get<std::string>(test);
	const char* see3 = field3.Get<const char*>(test);

	std::string str = "PI = ";

	method1.InvokeEasy<void, IReflectable &, std::string &, float>(test, str, 3.14f);
	int ret = method2.InvokeEasy<int, IReflectable &, const char *>(test, "Hello Reflected");
	printf("Main(): Return from Foo2 = %d\n", ret);

	constMethod.Invoke(test);
	printf("Main(): Return from FooConst = %s\n", constMethod.GetRetVal<std::string>().c_str());

	field1.Set(test, 13);
	printf("Main(): str = %s\n", str.c_str());

	//rvalMethod.Invoke(Test());
}
