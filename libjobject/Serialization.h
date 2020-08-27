#pragma once

#include "Object.h"
#include "TypeInfo.h"
#include "../libjson/include/json/json.h"
#include "exceptions/SerializationException.h"
#include "exceptions/DeserializationException.h"
#include <optional>
#include <vector>
#include <list>

template <typename Type>
class Serialization {
public:
	static Json::Value Serialize(Type val) {
		if constexpr (std::is_base_of_v<Object, Type>) {
			return val.serialize();
		}
		else {
			throw SerializationException(TypeInfo<Type>::getName());
		}
	}

	static Type Deserialize(Json::Value val) {
		if constexpr (std::is_base_of_v<Object, Type>) {
			static_assert(std::is_default_constructible_v<Type>, "Type is not default constructible");
			Type v;
			v.deserialize(val);
			return v;
		}
		else {
			throw DeserializationException(TypeInfo<Type>::getName());
		}
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
class Serialization<std::list<Type>> {
public:
	static Json::Value Serialize(std::list<Type> vec) {
		Json::Value::ArrayIndex index = 0;
		Json::Value ret;
		for (auto it = vec.begin(); it != vec.end(); it++) {
			ret.insert(index++, Serialization<Type>::Serialize(*it));
		}
		return ret;
	}

	static std::list<Type> Deserialize(Json::Value val) {
		size_t index = 0;
		std::list<Type> ret;
		for (auto it = val.begin(); it != val.end(); it++) {
			ret.push_back(Serialization<Type>::Deserialize(*it));
		}
		return ret;
	}
};

template <typename KeyType, typename ValueType>
class Serialization<std::map<KeyType, ValueType>> {
public:
	static Json::Value Serialize(std::map<KeyType, ValueType> m) {
		Json::Value::ArrayIndex index = 0;
		Json::Value ret;
		for (auto it = m.begin(); it != m.end(); it++) {
			Json::Value item;
			item["key"] = Serialization<KeyType>::Serialize(it->first);
			item["value"] = Serialization<ValueType>::Serialize(it->second);
			ret.insert(index++, item);
		}
		return ret;
	}

	static std::map<KeyType, ValueType> Deserialize(Json::Value val) {
		size_t index = 0;
		std::map<KeyType, ValueType> ret;
		for (auto it = val.begin(); it != val.end(); it++) {
			KeyType key = Serialization<KeyType>::Deserialize((*it)["key"]);
			ValueType value = Serialization<ValueType>::Deserialize((*it)["value"]);
			ret[key] = value;
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
class Serialization<bool> {
public:
	static Json::Value Serialize(bool value) {
		Json::Value json(value);
		return json;
	}

	static bool Deserialize(Json::Value json) {
		try {
			return json.asBool();
		}
		catch (const std::exception&) {
			throw DeserializationException(TypeInfo<bool>::getName());
		}
	}
};

template <>
class Serialization<char> {
public:
	static Json::Value Serialize(char value) {
		Json::Value json(value);
		return json;
	}

	static char Deserialize(Json::Value json) {
		try {
			return json.asInt();
		}
		catch (const std::exception&) {
			throw DeserializationException(TypeInfo<char>::getName());
		}
	}
};

template <>
class Serialization<unsigned char> {
public:
	static Json::Value Serialize(unsigned char value) {
		Json::Value json(value);
		return json;
	}

	static unsigned char Deserialize(Json::Value json) {
		try {
			return json.asUInt();
		}
		catch (const std::exception&) {
			throw DeserializationException(TypeInfo<unsigned char>::getName());
		}
	}
};

template <>
class Serialization<short> {
public:
	static Json::Value Serialize(short value) {
		Json::Value json(value);
		return json;
	}

	static short Deserialize(Json::Value json) {
		try {
			return json.asInt();
		}
		catch (const std::exception&) {
			throw DeserializationException(TypeInfo<short>::getName());
		}
	}
};

template <>
class Serialization<unsigned short> {
public:
	static Json::Value Serialize(unsigned short value) {
		Json::Value json(value);
		return json;
	}

	static unsigned short Deserialize(Json::Value json) {
		try {
			return json.asUInt();
		}
		catch (const std::exception&) {
			throw DeserializationException(TypeInfo<unsigned short>::getName());
		}
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
		try {
			return json.asInt();
		}
		catch (const std::exception&) {
			throw DeserializationException(TypeInfo<int>::getName());
		}
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
		try {
			return json.asUInt();
		}
		catch (const std::exception&) {
			throw DeserializationException(TypeInfo<unsigned int>::getName());
		}
	}
};

template <>
class Serialization<long> {
public:
	static Json::Value Serialize(long value) {
		Json::Value json(value);
		return json;
	}

	static long Deserialize(Json::Value json) {
		try {
			return json.asInt();
		}
		catch (const std::exception&) {
			throw DeserializationException(TypeInfo<long>::getName());
		}
	}
};

template <>
class Serialization<unsigned long> {
public:
	static Json::Value Serialize(unsigned long value) {
		Json::Value json((unsigned int)value);
		return json;
	}

	static unsigned long Deserialize(Json::Value json) {
		try {
			return json.asUInt();
		}
		catch (const std::exception&) {
			throw DeserializationException(TypeInfo<unsigned long>::getName());
		}
	}
};

template <>
class Serialization<long long> {
public:
	static Json::Value Serialize(long long value) {
		Json::Value json((Json::Int64)value);
		return json;
	}

	static long long Deserialize(Json::Value json) {
		try {
			return json.asInt64();
		}
		catch (const std::exception&) {
			throw DeserializationException(TypeInfo<long long>::getName());
		}
	}
};

template <>
class Serialization<unsigned long long> {
public:
	static Json::Value Serialize(unsigned long long value) {
		Json::Value json((Json::UInt64)value);
		return json;
	}

	static unsigned long long Deserialize(Json::Value json) {
		try {
			return json.asUInt64();
		}
		catch (const std::exception&) {
			throw DeserializationException(TypeInfo<long long>::getName());
		}
	}
};

template <>
class Serialization<float> {
public:
	static Json::Value Serialize(float value) {
		Json::Value json(value);
		return json;
	}

	static float Deserialize(Json::Value json) {
		try {
			return json.asFloat();
		}
		catch (const std::exception&) {
			throw DeserializationException(TypeInfo<float>::getName());
		}
	}
};

template <>
class Serialization<double> {
public:
	static Json::Value Serialize(double value) {
		Json::Value json(value);
		return json;
	}

	static double Deserialize(Json::Value json) {
		try {
			return json.asDouble();
		}
		catch (const std::exception&) {
			throw DeserializationException(TypeInfo<double>::getName());
		}
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
		try {
			return json.asString();
		}
		catch (const std::exception&) {
			throw DeserializationException(TypeInfo<std::string>::getName());
		}
	}
};
