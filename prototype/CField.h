#pragma once

#include "IReflectable.h"
#include "CAdaptor.h"

template<typename Class, typename Type>
class CField : public IField {
private:
	Type Class::*m_ptr;

public:
	CField(Type Class::* ptr) : m_ptr(ptr) {
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
