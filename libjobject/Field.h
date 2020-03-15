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

	const char* GetName() const {
		return m_field.GetName();
	}

	template<typename Type>
	Type Get(const Object &obj) const {
		CAdaptor<Type>& adaptor = static_cast<CAdaptor<Type>&>(m_field.GetValue(obj));
		return adaptor.GetValue();
	}

	template<typename Type>
	void Set(Object &obj, Type value) const {
		CAdaptor<Type> adaptor(value);
		m_field.SetValue(obj, adaptor);
	}

	std::string Serialize(const Object& obj) const {
		Json::Value json = m_field.Serialize(obj);
		Json::StreamWriterBuilder wbuilder;
		wbuilder["indentation"] = "\t";
		return Json::writeString(wbuilder, json);
	}

	void Deserialize(Object &obj, std::string val) const {
		Json::CharReaderBuilder rbuilder;
		rbuilder["collectComments"] = false;
		std::string errs;
		std::stringstream s(val);
		Json::Value json;
		Json::parseFromStream(rbuilder, s, &json, &errs);
		m_field.Deserialize(obj, json);
	}
};
