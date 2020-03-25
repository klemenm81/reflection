#pragma once

#include "../libjson/include/json/json.h"
#include "exceptions/SerializationException.h"
#include "exceptions/DeserializationException.h"
#include <optional>

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

template <typename Type>
class Serialization<std::optional<Type>> {
public:
	static Json::Value Serialize(std::optional<Type> value) {
		if (value.has_value()) {
			return Serialization<Type>::Serialize(value.value());
		}
		else {
			return Json::nullValue;
		}
	}

	static std::optional<Type> Deserialize(Json::Value json) {
		std::optional<Type> ret;
		if (json != Json::nullValue) {
			ret.emplace(Serialization<Type>::Deserialize(json));
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

template <>
class Serialization<std::string> {
public:
	static Json::Value Serialize(std::string value) {
		Json::Value json(value);
		return json;
	}

	static std::string Deserialize(Json::Value json) {
		return json.asString();
	}
};