#pragma once

#include "Object.h"
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
		if (std::is_base_of_v<Object, Type>) {

		}
		else {
			throw SerializationException(typeid(Type).name());
		}
	}

	static Type Deserialize(Json::Value val) {
		if (std::is_base_of_v<Object, Type>) {

		}
		else {
			throw DeserializationException(typeid(Type).name());
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
		return json.asBool();
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
		return json.asInt();
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
		return json.asUInt();
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
		return json.asInt();
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
		return json.asUInt();
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
class Serialization<long> {
public:
	static Json::Value Serialize(long value) {
		Json::Value json(value);
		return json;
	}

	static long Deserialize(Json::Value json) {
		return json.asInt();
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
		return json.asUInt();
	}
};

template <>
class Serialization<long long> {
public:
	static Json::Value Serialize(long long value) {
		Json::Value json(value);
		return json;
	}

	static long long Deserialize(Json::Value json) {
		return json.asInt64();
	}
};

template <>
class Serialization<unsigned long long> {
public:
	static Json::Value Serialize(unsigned long long value) {
		Json::Value json(value);
		return json;
	}

	static unsigned long long Deserialize(Json::Value json) {
		return json.asUInt64();
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
		return json.asFloat();
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
		return json.asDouble();
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