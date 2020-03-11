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
#include "Class.h"
#include "IClassRegistry.h"

#include "test.h"

template <typename... Adaptors>
std::vector<IAdaptor*> BuildAdaptorVectorFromArgs(Adaptors&&... adaptors) {
	std::vector<IAdaptor*> result;
	if constexpr (sizeof...(Adaptors) > 0) {
		int dummy[] = { (result.push_back(&adaptors), 0)... };
	}
	return result;
}

template <typename... Args>
Object& CreateInstance(const char* name, Args... args) {
#ifdef _WIN32
	static HMODULE hModule = GetModuleHandle(NULL);
	IClassRegistry& (*ClassRegistry)() =
		(IClassRegistry & (*)())GetProcAddress(hModule, "ClassRegistry");
#else
	void *hModule = dlopen(NULL, RTLD_NOW | RTLD_LOCAL);
	if (hModule == NULL) {
		perror("dlopen");
		exit(1);
	}
	IClassRegistry& (*ClassRegistry)() =
                (IClassRegistry & (*)())dlsym(hModule, "ClassRegistry");
	if (ClassRegistry == NULL) {
		printf("ClassRegistry not found\n");
		exit(1);
	}
#endif
	IClassRegistry& classRegistry = ClassRegistry();
	Class clasz = classRegistry.GetClass("Test");
	return clasz.Instantiate<>();
}



int main() {
	try {
 		Object& test = CreateInstance<int>("Test", 5);

		Field field1 = test.GetClass().GetField("a");
		Field field2 = test.GetClass().GetField("myString");
		Field field3 = test.GetClass().GetField("ptrString");
		Field field4 = test.GetClass().GetField("vec1");
		Method method1 = test.GetClass().GetMethod("Foo1");
		Method method2 = test.GetClass().GetMethod("Foo2");
		Method constMethod = test.GetClass().GetMethod("FooConst");
		Method rvalMethod = test.GetClass().GetMethod("Bar17");
		Method overloadedMethod = test.GetClass().GetMethod("FooOverloaded");

		int see1 = field1.Get<int>(test);
		std::string see2 = field2.Get<std::string>(test);
		const char* see3 = field3.Get<const char*>(test);

		std::string ser1 = field1.Serialize(test);
		field1.Deserialize(test, "15");

		std::string ser2 = field4.Serialize(test);
		field4.Deserialize(test, "[15, 14, 12]");

		std::string str = "PI = ";

		method1.InvokeInline<void, std::string&, float>(test, str, 3.14f);
		int ret = method2.InvokeInline<int, const char*>(test, "Hello Reflected");
		printf("Main(): Return from Foo2 = %d\n", ret);

		method1.PushArg<std::string&>(str);
		method1.PushArg<float>(3.14f);
		method1.Invoke(test);

		method2.PushArg<const char*>("Hello Reflected");
		method2.Invoke(test);
		int ret2 = method2.GetRetVal<int>();

		std::string retStr = overloadedMethod.InvokeMarshalled(test, std::vector<std::string> { "[3, 5, 4]", "5" });

		//constMethod.Invoke(test);
		//printf("Main(): Return from FooConst = %s\n", constMethod.GetRetVal<std::string>().c_str());

		field1.Set(test, 13);
		printf("Main(): str = %s\n", str.c_str());

		rvalMethod.Invoke(Test());
	}
	catch (const Exception& e) {
		printf("ERROR: Exception caught: %s\n", e.Message());
	}
}
