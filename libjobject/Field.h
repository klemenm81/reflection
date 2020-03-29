#pragma once

#include "detail/CAdaptor.h"
#include "detail/IField.h"
#include "Object.h"
#include "../libjson/include/json/json.h"
#include <sstream>

class Field {
private:
	const IField& m_field;

public:
	Field(const IField& field) : m_field(field) {
	}

	Field(Field&& other) noexcept : m_field(other.m_field) {
	}

	Field(const Field& other) : m_field(other.m_field) {
	}

	Field& operator=(Field&&) = delete;
	Field& operator=(const Field&) = delete;

	const char* getName() const {
		return m_field.getName();
	}

	template<typename Type>
	Type get(const Object &obj) const {
		CAdaptor<Type>& adaptor = static_cast<CAdaptor<Type>&>(m_field.getValue(obj));
		return adaptor.getValue();
	}

	template<typename Type>
	void set(Object &obj, Type value) const {
		CAdaptor<Type> adaptor(value);
		m_field.setValue(obj, adaptor);
	}

	Json::Value serialize(const Object& obj) const {
		return(m_field.serialize(obj));
	}

	const char *toString(const Object& obj) const {
		static thread_local std::string ret;
		Json::Value json = serialize(obj);
		Json::StreamWriterBuilder wbuilder;
		wbuilder["indentation"] = "\t";
		ret = Json::writeString(wbuilder, json);
		return ret.c_str();
	}

	void deserialize(Object& obj, Json::Value json) const {
		m_field.deserialize(obj, json);
	}

	void fromString(Object &obj, const char *val) const {
		Json::CharReaderBuilder rbuilder;
		rbuilder["collectComments"] = false;
		std::string errs;
		std::stringstream s(val);
		Json::Value json;
		bool result = Json::parseFromStream(rbuilder, s, &json, &errs);
		if (!result) {
			std::stringstream s(std::string("\"") + val + std::string("\""));
			bool result = Json::parseFromStream(rbuilder, s, &json, &errs);
		}
		deserialize(obj, json);
	}

	Indirection getIndirection() {
		return m_field.getIndirection();
	}

	template<typename Type>
	bool isType() {
		if (std::to_string(typeid(Type).hash_code()) == m_field.getTypeSignature()) {
			return true;
		}
		else {
			return false;
		}
	}

	bool isOptional() {
		return m_field.isOptional();
	}
};
