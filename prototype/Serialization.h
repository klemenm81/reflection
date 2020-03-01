#pragma once

#include "json/json.h"
#include "exceptions/SerializationException.h"
#include "exceptions/DeserializationException.h"

template <typename Type>
class Serialization {
public:
	static Json::Value Serialize(Type val) {
		throw SerializationException(typeid(Type).name());
	}

	static Type Deserialize(Json::Value val) {
		throw DeserializationException(typeid(Type).name());
	}
};

template <typename Type>
class Serialization<std::vector<Type>> {
public:
	static Json::Value Serialize(std::vector<Type> vec) {
		Json::Value::ArrayIndex index = 0;
		Json::Value ret;
		for (auto it = vec.begin(); it != vec.end(); it++) {
			ret.insert(index++, Serialization<Type>::Serialize(*it));
		}
		return ret;
	}

	static std::vector<Type> Deserialize(Json::Value val) {
		size_t index = 0;
		std::vector<Type> ret;
		for (auto it = val.begin(); it != val.end(); it++) {
			ret.push_back(Serialization<Type>::Deserialize(*it));
		}
		return ret;
	}
};

template <>
class Serialization<int> {
public:
	static Json::Value Serialize(int value) {
		Json::Value json(value);
		return json;
	}

	static int Deserialize(Json::Value json) {
		return json.asInt();
	}
};

template <>
class Serialization<unsigned int> {
public:
	static Json::Value Serialize(unsigned int value) {
		Json::Value json(value);
		return json;
	}

	static unsigned int Deserialize(Json::Value json) {
		return json.asUInt();
	}
};