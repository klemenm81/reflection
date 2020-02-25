#pragma once

#include "IField.h"
#include "CAdaptor.h"
#include "Object.h"

template<typename Class, typename Type>
class CFieldBase : public IField {
private:
	Type Class::*m_ptr;

public:
	constexpr CFieldBase(Type Class::* ptr) : m_ptr(ptr) {
	}

	IAdaptor& GetValue(const Object& obj) {
		static thread_local std::byte retValBuffer[sizeof(CAdaptor<Type>)];
		CAdaptor<Type>* adaptor = new(retValBuffer) CAdaptor<Type>(static_cast<const Class&>(obj).*m_ptr);
		return *adaptor;
	}

	void SetValue(Object& obj, IAdaptor& value) {
		CAdaptor<Type>& adaptor = static_cast<CAdaptor<Type>&>(value);
		static_cast<Class&>(obj).*m_ptr = adaptor.GetValue();
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