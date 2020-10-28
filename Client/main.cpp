#include <stdio.h>
#include <errno.h>

#include "Object.h"
#include "Field.h"
#include "Method.h"
#include "Class.h"
#include "ClassRegistry.h"
#include "Reflectable.h"

#include "../components/CLIParser/IParser.h"

#include "example.h"

#include "ParseStruct.h"

#include <iostream>
#include <signal.h>

#include <condition_variable>
#include <mutex>
#include <chrono>

using namespace std;

condition_variable m_condition;
mutex m_mutex;


void handleUserInterrupt(int signal) {
	if (signal == SIGINT) {
		m_condition.notify_one();
	}
}

void waitUntilUserInterrupt() {
	unique_lock<mutex> lock{ m_mutex };
	m_condition.wait(lock);
	lock.unlock();
}

struct CustomType {
	string strValue;
	float floatValue;
	int intValue;
};

template <typename T>
struct CustomContainer : public vector<T>
{
};

template <>
class Serialization<CustomType> {
public:
	static Json::Value Serialize(CustomType customType) {
		Json::Value::ArrayIndex index = 0;
		Json::Value ret;
		ret["strValue"] = Serialization<string>::Serialize(customType.strValue);
		ret["floatValue"] = Serialization<float>::Serialize(customType.floatValue);
		ret["intValue"] = Serialization<int>::Serialize(customType.intValue);
		return ret;
	}

	static CustomType Deserialize(Json::Value val) {
		size_t index = 0;
		CustomType ret;
		ret.strValue = Serialization<string>::Deserialize(val["strValue"]);
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
			cout << clasz.getName() << "\n";
		}

		ExampleStruct exampleStructInstance;
		exampleStructInstance.mainValue = 1;
		exampleStructInstance.values["A"] = { 1, 2, 3 };

		Class exampleClass = registry.getClass("ExampleClass");

		for (Field field : exampleStructInstance.getClass().getFields()) {
			cout << field.getName() << endl;
		}
		for (Method method : exampleClass.getMethods()) {
			cout << method.getName() << endl;
		}

		exampleStructInstance.getClass().getField("mainValue").set(exampleStructInstance, 2);

		unique_ptr<Object> exampleClassInstance =
			exampleClass.newInstance(string("B"), vector<int>{4, 5, 6});

		Method setFactorMethod = exampleClass.getMethod("setFactor");
		setFactorMethod.invoke<void>(*exampleClassInstance, 2.0f);

		Method addMethod = exampleClass.getMethod("add");
		addMethod.invoke<void>(*exampleClassInstance, string("C"), vector<int>{7, 8, 9});
		addMethod.invoke<void, const ExampleStruct&>(*exampleClassInstance, exampleStructInstance);

		Method getValuesMethod = exampleClass.getMethod("getValues");
		vector<float> result = getValuesMethod.invoke<vector<float>>(*exampleClassInstance, "A");

		cout << exampleClassInstance->toString() << "\n";
	}
	catch (Exception &e) {
		cout << e.Message() << "\n";
	}
}

void example00() {
	try {
		ClassRegistry registry;
		Class exampleClass = registry.getClass("ExampleClass");
		unique_ptr<Object> exampleClassInstance = exampleClass.newInstance();
		string retVal;

		Method setFactorMethod = exampleClass.getMethod("setFactor");
		retVal = setFactorMethod.invokeSerialized(*exampleClassInstance, { "3.14159" } );

		Method addMethod = exampleClass.getMethod("add");
		retVal = addMethod.invokeSerialized(*exampleClassInstance, { "D", "[ 10, 11, 12]" });

		cout << exampleClassInstance->toString() << "\n";
	}
	catch (const Exception & e) {
		cout << e.Message() << "\n";
	}
}


int example1(int argc, char** argv) {
	try {
		ClassRegistry parserRegistry("CLIParser");
		Class parserClass = parserRegistry.getClass("Parser");
		unique_ptr<Object> obj = parserClass.newInstance();	
		IParser& parser = parserClass.upcast<IParser>(*obj);
		ParseStruct parseStruct;
		try {
			parser.parse(argc, argv, parseStruct);
		}
		catch (const IParseException & e) {
			cout << "Exception occurred: " << e.Message() << "\n";
			parser.printUsage(parseStruct);
			return 1;
		}

		cout << parseStruct.toString() << "\n";
		return 0;
	}
	catch (const Exception & e) {
		cout << "Exception occurred: " << e.Message() << "\n";
		return 1;
	}
}


void example3() {
	try {
		ClassRegistry classRegistry("Test");									// Load Test.dll and acquire its class registry
		vector<Class> classes = classRegistry.getClasses();						// Get vector of all classes in Test.dll

		Class clasz = classRegistry.getClass("Test");								// Get metaobject of class Test
		unique_ptr<Object> test = clasz.newInstance<int>(5);						// Create an instance of Test with non-default constructor

		Field field1 = test->getClass().getField("a");
		Field field2 = test->getClass().getField("myString");
		Field field3 = test->getClass().getField("ptrString");
		Field field4 = test->getClass().getField("vec1");
		Method method1 = test->getClass().getMethod("foo1");
		Method method2 = test->getClass().getMethod("foo2");
		Method constMethod = test->getClass().getMethod("fooConst");
		Method rvalMethod = test->getClass().getMethod("bar17");
		Method overloadedMethod = test->getClass().getMethod("fooOverloaded");

		vector<Method> methods = test->getClass().getMethods();						// Get list of all methods of class Test

		int see1 = field1.get<int>(*test);											// Get field value of type int
		string see2 = field2.get<string>(*test);									// Get field value of type std::string
		const char* see3 = field3.get<const char*>(*test);							// Get field value of type const char *

		field1.set(*test, 13);														// Set field a of type int to value 13

		string ser1 = field1.toString(*test);										// Get serialized field value of a, which is of type int
		field1.fromString(*test, "15");												// Set field value of a by means of deserialization in json form

		string ser2 = field4.toString(*test);										// Get serialized value of vec1, which is of type std::vector<int>
		field4.fromString(*test, "[15, 14, 12]");									// Set field value of vec1 by means of deserialization in json form

		string str = "PI = ";

		method1.invoke<void, string&, float>(*test, str, 3.14f);					// Invoke method foo1 with parameters of type string& and float and void return
		int ret = method2.invoke<int, const char*>(*test, "Hello Reflected");		// Invoke method foo2 with parameters of type const char* and return of type int
		printf("Main(): Return from Foo2 = %d\n", ret);
		printf("Main(): str = %s\n", str.c_str());

		string retStr = overloadedMethod.invokeSerialized(							// Invoke method foo1 by not being aware of its input type and type of the return value
			*test,																	
			vector<string> { "[3, 5, 4]", "5" }										// Parameters are represented as strings in json form
		);

		Class testDerivedClass = classRegistry.getClass("TestDerived");				// Get metaobject of class TestDerived, which is a derived class from Test
		unique_ptr<Object> testDerived = testDerivedClass.newInstance();			// Create an instance of class TestDerived
	}
	catch (const Exception & e) {
		printf("ERROR: Exception caught: %s\n", e.Message());
	}
}

void testPerformance() {
	try {
		ClassRegistry classRegistry;
		Class clasz = classRegistry.getClass("ExampleClass");

		unique_ptr<Object> test = clasz.newInstance();

		Method m = clasz.getMethod("testPerformance");

		chrono::steady_clock::time_point begin = chrono::steady_clock::now();

		for (int i = 0; i < 10000000; i++) {
			double ret = m.invoke<double, string, vector<int>>(*test, "test", {1, 2, 3});
		}

		chrono::steady_clock::time_point end = chrono::steady_clock::now();

		cout << "Time difference = " << chrono::duration_cast<chrono::nanoseconds> (end - begin).count() << "[ns]" << "\n";
	}
	catch (const Exception & e) {
		printf("ERROR: Exception caught: %s\n", e.Message());
	}
}



int main(int argc, char **argv) {
	example0();
	example00();
	example1(argc, argv);
	example3();
	testPerformance();
}
