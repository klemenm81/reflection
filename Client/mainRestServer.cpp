#include <stdio.h>
#include <errno.h>

#include "Object.h"
#include "Field.h"
#include "Method.h"
#include "Class.h"
#include "ClassRegistry.h"
#include "Reflectable.h"

#include "../components/CLIParser/IParser.h"
#include "../components/RESTServer/IRESTController.h"

#include "example.h"

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

struct CustomType {
	std::string strValue;
	float floatValue;
	int intValue;
};

template <typename T>
struct CustomContainer : public std::vector<T>
{
};

template <>
class Serialization<CustomType> {
public:
	static Json::Value Serialize(CustomType customType) {
		Json::Value::ArrayIndex index = 0;
		Json::Value ret;
		ret["strValue"] = Serialization<std::string>::Serialize(customType.strValue);
		ret["floatValue"] = Serialization<float>::Serialize(customType.floatValue);
		ret["intValue"] = Serialization<int>::Serialize(customType.intValue);
		return ret;
	}

	static CustomType Deserialize(Json::Value val) {
		size_t index = 0;
		CustomType ret;
		ret.strValue = Serialization<std::string>::Deserialize(val["strValue"]);
		ret.floatValue = Serialization<float>::Deserialize(val["floatValue"]);
		ret.intValue = Serialization<int>::Deserialize(val["intValue"]);
		return ret;
	}
};

template <typename Type>
class Serialization<CustomContainer<Type>> {
public:
	static Json::Value Serialize(CustomContainer<Type> customContainer) {
		Json::Value::ArrayIndex index = 0;
		Json::Value ret;
		for (Type type : customContainer) {
			ret.insert(index++, Serialization<Type>::Serialize(type));
		}
		return ret;
	}

	static CustomContainer<Type> Deserialize(Json::Value val) {
		size_t index = 0;
		CustomContainer<Type> ret;
		for (auto it = val.begin(); it != val.end(); it++) {
			ret.push_back(Serialization<Type>::Deserialize(*it));
		}
		return ret;
	}
};


void example0() {
	try {
		ClassRegistry registry;

		for (Class clasz : registry.getClasses()) {
			std::cout << clasz.getName() << std::endl;
		}

		ExampleStruct exampleStructInstance;
		exampleStructInstance.mainValue = 1;
		exampleStructInstance.values["A"] = { 1, 2, 3 };

		Class exampleClass = registry.getClass("ExampleClass");

		for (Field field : exampleStructInstance.getClass().getFields()) {
			std::cout << field.getName() << std::endl;
		}
		for (Method method : exampleClass.getMethods()) {
			std::cout << method.getName() << std::endl;
		}

		exampleStructInstance.getClass().getField("mainValue").set(exampleStructInstance, 2);

		std::unique_ptr<Object> exampleClassInstance =
			exampleClass.newInstance(std::string("B"), std::vector<int>{4, 5, 6});

		Method setFactorMethod = exampleClass.getMethod("setFactor");
		setFactorMethod.invoke<void>(*exampleClassInstance, 2.0f);

		Method addMethod = exampleClass.getMethod("add");
		addMethod.invoke<void>(*exampleClassInstance, std::string("C"), std::vector<int>{7, 8, 9});
		addMethod.invoke<void, const ExampleStruct&>(*exampleClassInstance, exampleStructInstance);

		Method getValuesMethod = exampleClass.getMethod("getValues");
		std::vector<float> result = getValuesMethod.invoke<std::vector<float>>(*exampleClassInstance, "A");

		std::cout << exampleClassInstance->toString() << std::endl;
	}
	catch (Exception &e) {
		std::cout << e.Message() << std::endl;
	}
}

void example00() {
	try {
		ClassRegistry registry;
		Class exampleClass = registry.getClass("ExampleClass");
		std::unique_ptr<Object> exampleClassInstance = exampleClass.newInstance();
		std::string retVal;

		Method setFactorMethod = exampleClass.getMethod("setFactor");
		retVal = setFactorMethod.invokeSerialized(*exampleClassInstance, { "3.14159" } );

		Method addMethod = exampleClass.getMethod("add");
		retVal = addMethod.invokeSerialized(*exampleClassInstance, { "D", "[ 10, 11, 12]" });

		std::cout << exampleClassInstance->toString() << std::endl;
	}
	catch (Exception & e) {
		std::cout << e.Message() << std::endl;
	}
}


int example1(int argc, char** argv) {
	try {
		ClassRegistry parserRegistry("CLIParser.dll");
		Class parserClass = parserRegistry.getClass("Parser");
		std::unique_ptr<Object> obj = parserClass.newInstance();	
		IParser& parser = parserClass.upcast<IParser>(*obj);
		ParseStruct parseStruct;
		try {
			parser.parse(argc, argv, parseStruct);
		}
		catch (const IParseException & e) {
			printf("Exception occurred: %s\n", e.Message());
			parser.printUsage(parseStruct);
			return 1;
		}

		std::cout << parseStruct.toString() << "\n";
	}
	catch (const Exception & e) {
		printf("Exception occurred: %s\n", e.Message());
	}

	return 0;
}


void example2() {
	try {
		MyRestServer myRestServer;
		ClassRegistry restServerRegistry("RESTServer.dll");
		Class restControllerClass = restServerRegistry.getClass("RESTController");
		std::unique_ptr<Object> obj =
			restControllerClass.newInstance<std::wstring, Object&>(
				L"http://localhost:6502/v1/reflection/api", 
				myRestServer
			);

		IRESTController& restController = restControllerClass.upcast<IRESTController>(*obj);
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
		ClassRegistry classRegistry("Test.dll");									// Load Test.dll and acquire its class registry
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

		std::string retStr = overloadedMethod.invokeSerialized(						// Invoke method foo1 by not being aware of its input type and type of the return value
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
		ClassRegistry classRegistry;
		Class clasz = classRegistry.getClass("ExampleClass");

		std::unique_ptr<Object> test = clasz.newInstance();

		Method m = clasz.getMethod("testPerformance");

		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

		for (int i = 0; i < 10000000; i++) {
			double ret = m.invoke<double, std::string, std::vector<int>>(*test, "test", {1, 2, 3});
		}

		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

		std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;
	}
	catch (const Exception & e) {
		printf("ERROR: Exception caught: %s\n", e.Message());
	}
}



int main(int argc, char **argv) {
	example0();
	example00();
	example1(argc, argv);
	//example2();
	example3();
	testPerformance();
}
