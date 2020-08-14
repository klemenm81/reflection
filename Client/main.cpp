#include <stdio.h>
#include <errno.h>

#include "../libjobject/Object.h"
#include "../libjobject/Field.h"
#include "../libjobject/Method.h"
#include "../libjobject/Class.h"
#include "../libjobject/ClassRegistry.h"
#include "../libjobject/Reflectable.h"

#include "../components/CLIParser/IParser.h"
#include "../components/RESTServer/IRESTController.h"

#include "ParseStruct.h"
#include "MyRestServer.h"

#include <iostream>
#include <signal.h>

#include <condition_variable>
#include <mutex>
#include <chrono>

std::condition_variable m_condition;
std::mutex m_mutex;

void handleUserInterrupt(int signal) {
	if (signal == SIGINT) {
		m_condition.notify_one();
	}
}

void waitUntilUserInterrupt() {
	std::unique_lock<std::mutex> lock{ m_mutex };
	m_condition.wait(lock);
	lock.unlock();
}


void example1(int argc, char** argv) {
	try {
		ClassRegistry parserRegistry = ClassRegistry::GetClassRegistry("CLIParser.dll");// Load CLIParser.dll and acquire its class registry
		Class parserClass = parserRegistry.getClass("Parser");							// Get metaobject of class Parser
		std::unique_ptr<Object> obj(parserClass.newInstance());							// Create instance of Parser and store its pointer to unique_ptr
		IParser& parser = parserClass.upcast<IParser>(*obj);							// Acquire IParser interface from the created instance
		ParseStruct parseStruct;		
		parser.parse(argc, argv, parseStruct);											// Parse argc/argv pair and store the values in parseStruct

		const char *serialized = parseStruct.toString();
		std::cout << serialized << "\n";
		parseStruct.fromString(serialized);
	}
	catch (const Exception & e) {
		printf("Exception occured during parse of options: %s\n", e.Message());
	}
}


void example2() {
	try {
		MyRestServer myRestServer;
		ClassRegistry parserRegistry = ClassRegistry::GetClassRegistry("RESTServer.dll");
		Class parserClass = parserRegistry.getClass("RESTController");
		std::unique_ptr<Object> obj(parserClass.newInstance<std::wstring, Object&>(L"http://localhost:6502/v1/reflection/api", myRestServer));
		IRESTController& restController = parserClass.upcast<IRESTController>(*obj);

		signal(SIGINT, handleUserInterrupt);

		restController.start();

		waitUntilUserInterrupt();

		restController.shutdown();
	}
	catch (const Exception & e) {
		printf("Exception occured during parse of options: %s\n", e.Message());
	}
}


void example3() {
	try {
		ClassRegistry classRegistry = ClassRegistry::GetClassRegistry("Test.dll");	// Load Test.dll and acquire its class registry
		std::vector<Class> classes = classRegistry.getClasses();					// Get vector of all classes in Test.dll

		Class clasz = classRegistry.getClass("Test");								// Get metaobject of class Test
		std::unique_ptr<Object> test = clasz.newInstance<int>(5);					// Create an instance of Test with non-default constructor

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

		std::string ser1 = field1.toString(*test);									// Get serialized field value of a, which is of type int
		field1.fromString(*test, "15");												// Set field value of a by means of deserialization in json form

		std::string ser2 = field4.toString(*test);									// Get serialized value of vec1, which is of type std::vector<int>
		field4.fromString(*test, "[15, 14, 12]");									// Set field value of vec1 by means of deserialization in json form

		std::string str = "PI = ";

		method1.invoke<void, std::string&, float>(*test, str, 3.14f);			// Invoke method foo1 with parameters of type string& and float and void return
		int ret = method2.invoke<int, const char*>(*test, "Hello Reflected");	// Invoke method foo2 with parameters of type const char* and return of type int
		printf("Main(): Return from Foo2 = %d\n", ret);
		printf("Main(): str = %s\n", str.c_str());

		std::string retStr = overloadedMethod.invokeMarshalled(						// Invoke method foo1 by not being aware of its input type and type of the return value
			*test,																	
			std::vector<std::string> { "[3, 5, 4]", "5" }							// Parameters are represented as strings in json form
		);

		Class testDerivedClass = classRegistry.getClass("TestDerived");				// Get metaobject of class TestDerived, which is a derived class from Test
		std::unique_ptr<Object> testDerived = testDerivedClass.newInstance();		// Create an instance of class TestDerived
	}
	catch (const Exception & e) {
		printf("ERROR: Exception caught: %s\n", e.Message());
	}
}

void testPerformance() {
	try {
		ClassRegistry classRegistry = ClassRegistry::GetClassRegistry("Test.dll");
		std::vector<Class> classes = classRegistry.getClasses();
		Class clasz = classRegistry.getClass("Test");

		std::unique_ptr<Object> test = clasz.newInstance<int>(5);

		Method m = clasz.getMethod("testPerformance");

		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

		for (int i = 0; i < 10000000; i++) {
			std::string str = m.invoke<std::string, std::string, int, double>(*test, "test", 5, 3.14);
		}

		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

		std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;
	}
	catch (const Exception & e) {
		printf("ERROR: Exception caught: %s\n", e.Message());
	}
}

int main(int argc, char **argv) {
	example1(argc, argv);
	//example2();
	example3();
	testPerformance();
}
