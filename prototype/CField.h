#pragma once

#include "IField.h"
#include "CAdaptor.h"

class IReflectable;

template<typename Class, typename Type>
class CFieldBase : public IField {
private:
	Type Class::*m_ptr;

public:
	CFieldBase(Type Class::* ptr) : m_ptr(ptr) {
	}

	IAdaptor &GetValue(IReflectable &obj) {
		CAdaptor<Type>* adaptor = new CAdaptor<Type>(static_cast<Class&>(obj).*m_ptr);
		return *adaptor;
	}

	void SetValue(IReflectable &obj, IAdaptor &value) {
		CAdaptor<Type>& adaptor = static_cast<CAdaptor<Type> &>(value);
		static_cast<Class &>(obj).*m_ptr = adaptor.GetValue();
	}
};

template <typename Class, typename Field>
class CField;

template <typename Class, typename Type>
class CField<Class, Type Class::*> : public CFieldBase<Class, Type> {
public:
	CField(Type Class::* ptr) : CFieldBase<Class, Type>(ptr) {
	}
};