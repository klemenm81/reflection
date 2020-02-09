#pragma once

#include "Adaptor.h"
#include "IReflectable.h"
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

	template<typename Type, typename Class>
	Type Get(Class &obj) {
		Adaptor objectAdaptor = *new CAdaptor<Class &>(obj);

		std::byte retValBuffer[sizeof(CAdaptor<Type>)];
		Adaptor adaptor = m_field.GetValue(retValBuffer, objectAdaptor);
		return adaptor.Get<Type>();
	}

	template<typename Type, typename Class>
	void Set(Class &obj, Type value) {
		Adaptor objectAdaptor = *new CAdaptor<Class &>(obj);
		Adaptor adaptor = *new CAdaptor<Type>(value);
		m_field.SetValue(objectAdaptor, adaptor);
	}
};
