#pragma once

#include "IField.h"
#include "CAdaptor.h"

class IReflectable;

template<typename Class, typename Type>
class CFieldBase : public IField {
private:
	Type Class::*m_ptr;

public:
	constexpr CFieldBase(Type Class::* ptr) : m_ptr(ptr) {
	}

	IAdaptor& GetValue(IAdaptor& obj) {
		CAdaptor<Type>* adaptor = new CAdaptor<Type>(static_cast<CAdaptor<Class&>&>(obj).GetValue().*m_ptr);
		return *adaptor;
	}

	void SetValue(IAdaptor& obj, IAdaptor& value) {
		CAdaptor<Type>& adaptor = static_cast<CAdaptor<Type>&>(value);
		static_cast<CAdaptor<Class&>&>(obj).GetValue().*m_ptr = adaptor.GetValue();
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