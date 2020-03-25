#ifdef _WIN32
    #include <Windows.h>
#else
    #include <dlfcn.h>
#endif

#include <stdio.h>
#include <errno.h>

#include "../libjobject/Object.h"
#include "../libjobject/Field.h"
#include "../libjobject/Method.h"
#include "../libjobject/Class.h"
#include "../libjobject/ClassRegistry.h"
#include "../libjobject/Reflectable.h"

#include "../components/CLIParser/IParser.h"


ClassRegistry GetClassRegistry(const char *name) {
#ifdef _WIN32
	HMODULE hModule = LoadLibraryA(name);
	const IClassRegistry& (*ClassRegistryFcn)() =
		(const IClassRegistry & (*)())GetProcAddress(hModule, "ClassRegistry");
#else
	void *hModule = dlopen(NULL, RTLD_NOW | RTLD_LOCAL);
	if (hModule == NULL) {
		perror("dlopen");
		exit(1);
	}
	const IClassRegistry& (*ClassRegistryFcn)() =
                (const IClassRegistry & (*)())dlsym(hModule, "ClassRegistry");
	if (ClassRegistryFcn == NULL) {
		printf("ClassRegistry not found\n");
		exit(1);
	}
#endif
	ClassRegistry classRegistry = ClassRegistryFcn();
	return classRegistry;
}

#include "ParseStruct.h"

int main(int argc, char **argv) {
	try {
		ClassRegistry parserRegistry = GetClassRegistry("CLIParser.dll");
		Class parserClass = parserRegistry.getClass("Parser");
		std::unique_ptr<Object> obj(parserClass.newInstance());
		IParser& parser = parserClass.upcast<IParser>(*obj);
		ParseStruct parseStruct;
		parser.parse(argc, argv, parseStruct);
		
	}
	catch (const Exception & e) {
		printf("Exception occured during parse of options: %s\n", e.Message());
	}


	try {
		ClassRegistry classRegistry = GetClassRegistry("Test.dll");
		std::vector<Class> classes = classRegistry.getClasses();

		Class clasz = classRegistry.getClass("Test");
 		std::unique_ptr<Object> test(clasz.newInstance<int>(5));

		Field field1 = test->getClass().getField("a");
		Field field2 = test->getClass().getField("myString");
		Field field3 = test->getClass().getField("ptrString");
		Field field4 = test->getClass().getField("vec1");
		Method method1 = test->getClass().getMethod("foo1");
		Method method2 = test->getClass().getMethod("foo2");
		Method constMethod = test->getClass().getMethod("fooConst");
		Method rvalMethod = test->getClass().getMethod("bar17");
		Method overloadedMethod = test->getClass().getMethod("fooOverloaded");

		std::vector<Method> methods = test->getClass().getMethods();

		int see1 = field1.get<int>(*test);
		std::string see2 = field2.get<std::string>(*test);
		const char* see3 = field3.get<const char*>(*test);

		std::string ser1 = field1.serialize(*test);
		field1.deserialize(*test, "15");

		std::string ser2 = field4.serialize(*test);
		field4.deserialize(*test, "[15, 14, 12]");

		std::string str = "PI = ";

		method1.invokeInline<void, std::string&, float>(*test, str, 3.14f);
		int ret = method2.invokeInline<int, const char*>(*test, "Hello Reflected");
		printf("Main(): Return from Foo2 = %d\n", ret);

		method1.pushArg<std::string&>(str);
		method1.pushArg<float>(3.14f);
		method1.invoke(*test);

		method2.pushArg<const char*>("Hello Reflected");
		method2.invoke(*test);
		int ret2 = method2.getRetVal<int>();

		std::string retStr = overloadedMethod.invokeMarshalled(*test, std::vector<std::string> { "[3, 5, 4]", "5" });

		//constMethod.invoke(test);
		//printf("Main(): Return from FooConst = %s\n", constMethod.GetRetVal<std::string>().c_str());

		field1.set(*test, 13);
		printf("Main(): str = %s\n", str.c_str());

		//rvalMethod.invoke(Test());

		Class testDerivedClass = classRegistry.getClass("TestDerived");
		std::unique_ptr<Object> testDerived(testDerivedClass.newInstance());
	}
	catch (const Exception& e) {
		printf("ERROR: Exception caught: %s\n", e.Message());
	}
}
