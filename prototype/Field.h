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

	template<typename Type>
	Type Get(IReflectable& obj) {
		Adaptor adaptor = m_field.GetValue(obj);
		return adaptor.Get<Type>();
	}

	template<typename Type>
	void Set(IReflectable& obj, Type value) {
		Adaptor adaptor = *new CAdaptor<Type>(value);
		m_field.SetValue(obj, adaptor);
	}
};
