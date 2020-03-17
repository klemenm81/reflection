#pragma once

#include "IField.h"
#include "CAdaptor.h"
#include "../Object.h"
#include "../../libjson/include/json/json.h"

template<typename Class, typename Type>
class CFieldBase : public IField {
private:
	std::string m_name;
	Type Class::*m_ptr;

public:
	constexpr CFieldBase(const char *name, Type Class::* ptr) : m_name(name), m_ptr(ptr) {
	}

	const char* GetName() const {
		return m_name.c_str();
	}

	IAdaptor& GetValue(const Object& obj) const {
		static thread_local std::byte retValBuffer[sizeof(CAdaptor<Type>)];
		CAdaptor<Type>* adaptor = new(retValBuffer) CAdaptor<Type>(static_cast<const Class&>(obj).*m_ptr);
		return *adaptor;
	}

	void SetValue(Object& obj, IAdaptor& value) const {
		CAdaptor<Type>& adaptor = static_cast<CAdaptor<Type>&>(value);
		static_cast<Class&>(obj).*m_ptr = adaptor.GetValue();
	}

	Json::Value Serialize(const Object& obj) const {
		IAdaptor& adaptor = GetValue(obj);
		return adaptor.Marshall();
	}

	void Deserialize(Object& obj, Json::Value value) const {
		static thread_local std::byte retValBuffer[sizeof(CAdaptor<Type>)];
		CAdaptor<Type>* adaptor = new(retValBuffer) CAdaptor<Type>(value);
		SetValue(obj, *adaptor);
	}
};

template <typename Class, typename Field>
class CField;

template <typename Class, typename Type>
class CField<Class, Type Class::*> : public CFieldBase<Class, Type> {
public:
	CField(const char *name, Type Class::* ptr) : CFieldBase<Class, Type>(name, ptr) {
	}
};