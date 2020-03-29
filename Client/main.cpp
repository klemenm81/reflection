#include <stdio.h>
#include <errno.h>

#include "../libjobject/Object.h"
#include "../libjobject/Field.h"
#include "../libjobject/Method.h"
#include "../libjobject/Class.h"
#include "../libjobject/ClassRegistry.h"
#include "../libjobject/Reflectable.h"

#include "../components/CLIParser/IParser.h"

#include "ParseStruct.h"


void example1(int argc, char** argv) {
	try {
		ClassRegistry parserRegistry = ClassRegistry::GetClassRegistry("CLIParser.dll");// Load CLIParser.dll and acquire its class registry
		Class parserClass = parserRegistry.getClass("Parser");							// Get metaobject of class Parser
		std::unique_ptr<Object> obj(parserClass.newInstance());							// Create instance of Parser and store its pointer to unique_ptr
		IParser& parser = parserClass.upcast<IParser>(*obj);							// Acquire IParser interface from the created instance
		ParseStruct parseStruct;		
		parser.parse(argc, argv, parseStruct);											// Parse argc/argv pair and store the values in parseStruct
	}
	catch (const Exception & e) {
		printf("Exception occured during parse of options: %s\n", e.Message());
	}
}


void example2() {
	try {
		ClassRegistry classRegistry = ClassRegistry::GetClassRegistry("Test.dll");	// Load Test.dll and acquire its class registry
		std::vector<Class> classes = classRegistry.getClasses();					// Get vector of all classes in Test.dll

		Class clasz = classRegistry.getClass("Test");								// Get metaobject of class Test
		std::unique_ptr<Object> test(clasz.newInstance<int>(5));					// Create an instance of Test with non-default constructor

		Field field1 = test->getClass().getField("a");
		Field field2 = test->getClass().getField("myString");
		Field field3 = test->getClass().getField("ptrString");
		Field field4 = test->getClass().getField("vec1");
		Method method1 = test->getClass().getMethod("foo1");
		Method method2 = test->getClass().getMethod("foo2");
		Method constMethod = test->getClass().getMethod("fooConst");
		Method rvalMethod = test->getClass().getMethod("bar17");
		Method overloadedMethod = test->getClass().getMethod("fooOverloaded");

		std::vector<Method> methods = test->getClass().getMethods();				// Get list of all methods of class Test

		int see1 = field1.get<int>(*test);											// Get field value of type int
		std::string see2 = field2.get<std::string>(*test);							// Get field value of type std::string
		const char* see3 = field3.get<const char*>(*test);							// Get field value of type const char *

		field1.set(*test, 13);														// Set field a of type int to value 13

		std::string ser1 = field1.serialize(*test);									// Get serialized field value of a, which is of type int
		field1.deserialize(*test, "15");											// Set field value of a by means of deserialization in json form

		std::string ser2 = field4.serialize(*test);									// Get serialized value of vec1, which is of type std::vector<int>
		field4.deserialize(*test, "[15, 14, 12]");									// Set field value of vec1 by means of deserialization in json form

		std::string str = "PI = ";

		method1.invokeInline<void, std::string&, float>(*test, str, 3.14f);			// Invoke method foo1 with parameters of type string& and float and void return
		int ret = method2.invokeInline<int, const char*>(*test, "Hello Reflected");	// Invoke method foo2 with parameters of type const char* and return of type int
		printf("Main(): Return from Foo2 = %d\n", ret);
		printf("Main(): str = %s\n", str.c_str());

		method1.pushArg<std::string&>(str);											// Invoke method foo1 by dynamically adding parameters of type std::string& and float 
		method1.pushArg<float>(3.14f);
		method1.invoke(*test);

		method2.pushArg<const char*>("Hello Reflected");							// Invoke method foo2 by dynamically adding parameters of type const char *
		method2.invoke(*test);														// and acquiring return value of type int
		int ret2 = method2.getRetVal<int>();

		std::string retStr = overloadedMethod.invokeMarshalled(						// Invoke method foo1 by not being aware of its input type and type of the return value
			*test,																	
			std::vector<std::string> { "[3, 5, 4]", "5" }							// Parameters are represented as strings in json form
		);

		//constMethod.invoke(test);
		//printf("Main(): Return from FooConst = %s\n", constMethod.GetRetVal<std::string>().c_str());
		//rvalMethod.invoke(Test());

		Class testDerivedClass = classRegistry.getClass("TestDerived");				// Get metaobject of class TestDerived, which is a derived class from Test
		std::unique_ptr<Object> testDerived(testDerivedClass.newInstance());		// Create an instance of class TestDerived
	}
	catch (const Exception & e) {
		printf("ERROR: Exception caught: %s\n", e.Message());
	}
}

int main(int argc, char **argv) {
	example1(argc, argv);
	example2();	
}
