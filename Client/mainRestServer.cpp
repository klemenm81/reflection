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




int main(int argc, char **argv) {
	example2();
}
