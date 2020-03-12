#pragma once

#include "CAdaptor.h"
#include "Object.h"
#include "IField.h"

#include "json/json.h"
#include <sstream>

class Field {
private:
	const IField& m_field;

public:
	Field(const IField& field) : m_field(field) {
	}

	Field(Field&& other) noexcept : m_field(other.m_field) {
	}

	Field(const Field &) = delete;
	Field& operator=(const Field&) = delete;

	const char* GetName() {
		return m_field.GetName();
	}

	template<typename Type>
	Type Get(const Object &obj) {
		CAdaptor<Type>& adaptor = static_cast<CAdaptor<Type>&>(m_field.GetValue(obj));
		return adaptor.GetValue();
	}

	template<typename Type>
	void Set(Object &obj, Type value) {
		CAdaptor<Type> adaptor(value);
		m_field.SetValue(obj, adaptor);
	}

	std::string Serialize(const Object& obj) {
		Json::Value json = m_field.Serialize(obj);
		Json::StreamWriterBuilder wbuilder;
		wbuilder["indentation"] = "\t";
		return Json::writeString(wbuilder, json);
	}

	void Deserialize(Object &obj, std::string val) {
		Json::CharReaderBuilder rbuilder;
		rbuilder["collectComments"] = false;
		std::string errs;
		std::stringstream s(val);
		Json::Value json;
		Json::parseFromStream(rbuilder, s, &json, &errs);
		m_field.Deserialize(obj, json);
	}
};
