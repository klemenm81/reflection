#pragma once

#include "CAdaptor.h"
#include "Object.h"
#include "IField.h"

class Field {
private:
	IField& m_field;

public:
	Field(IField& field) : m_field(field) {
	}

	Field(Field&& other) noexcept : m_field(other.m_field) {
	}

	Field(const Field &) = delete;
	Field& operator=(const Field&) = delete;

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
};
