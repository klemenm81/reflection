#pragma once

#include "Adaptor.h"
#include "IReflectable.h"
#include "IField.h"

class Field {
private:
	IField& m_field;

public:
	explicit Field(IField& field) : m_field(field) {
	}

	template<typename Type, typename Class>
	Type Get(Class &obj) {
		Adaptor objectAdaptor = *new CAdaptor<Class &>(obj);
		Adaptor adaptor = m_field.GetValue(objectAdaptor);
		return adaptor.Get<Type>();
	}

	template<typename Type, typename Class>
	void Set(Class &obj, Type value) {
		Adaptor objectAdaptor = *new CAdaptor<Class &>(obj);
		Adaptor adaptor = *new CAdaptor<Type>(value);
		m_field.SetValue(objectAdaptor, adaptor);
	}
};
