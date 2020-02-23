#ifdef _WIN32
    #include <Windows.h>
#else
    #include <dlfcn.h>
#endif

#include <stdio.h>
#include <errno.h>

#include "Object.h"
#include "Field.h"
#include "Method.h"
#include "IClass.h"
#include "test.h"

Object& CreateInstance(const char* name) {
#ifdef _WIN32
	static HMODULE hModule = GetModuleHandle(NULL);
	Object& (*AbstractFactory)(const char*) =
		(Object & (*)(const char*))GetProcAddress(hModule, "AbstractFactory");
#else
	void *hModule = dlopen(NULL, RTLD_NOW | RTLD_LOCAL);
	if (hModule == NULL) {
		perror("dlopen");
		exit(1);
	}
	Object& (*AbstractFactory)(const char*) =
                (Object & (*)(const char*))dlsym(hModule, "AbstractFactory");
	if (AbstractFactory == NULL) {
		printf("AbstractFactory not found\n");
		exit(1);
	}
#endif
	return AbstractFactory("Test");
}


int main() {
	Object &test = CreateInstance("Test");

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

	method1.InvokeNewInline<void, std::string &, float>(test, str, 3.14f);
	int ret = method2.InvokeNewInline<int, const char *>(test, "Hello Reflected");
	printf("Main(): Return from Foo2 = %d\n", ret);

	method1.PushArg<std::string&>(str);
	method1.PushArg<float>(3.14f);
	method1.InvokeNew(test);

	method2.PushArg<const char *>("Hello Reflected");
	method2.InvokeNew(test);
	ret = method2.GetRetVal<int>();


	std::vector<IAdaptor *> args;
	printf("Main(): Return from FooConst = %s\n", constMethod.Invoke(test, args).Get<std::string>().c_str());

	field1.Set(test, 13);
	printf("Main(): str = %s\n", str.c_str());

	printf("Main(): Method1 args signature = %s\n", method1.GetArgsSignature());
	printf("Main(): Method1 args name = %s\n", method1.GetArgsName());
	printf("Main(): Method2 args signature = %s\n", method2.GetArgsSignature());
	printf("Main(): Method2 args name = %s\n", method2.GetArgsName());

	printf("Main(): Method1 retval signature = %s\n", method1.GetRetValSignature());
	printf("Main(): Method1 retval name = %s\n", method1.GetRetValName());
	printf("Main(): Method2 retval signature = %s\n", method2.GetRetValSignature());
	printf("Main(): Method2 retval name = %s\n", method2.GetRetValName());

	//rvalMethod.Invoke(Test());
}
