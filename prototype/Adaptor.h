#pragma once

#include "CAdaptor.h"
#include "IAdaptor.h"

class Adaptor {
private:
	IAdaptor &m_adaptor;

public:
	Adaptor(IAdaptor &adaptor) : m_adaptor(adaptor) {
	}

	~Adaptor() {
		delete &m_adaptor;
	}

	template<typename Type>
	Type Get() {
		return(static_cast<CAdaptor<Type>&>(m_adaptor).GetValue());
	}

	template<typename Type>
	void Set(Type value) {
		static_cast<CAdaptor<Type>&>(m_adaptor).SetValue(value);
	}

	operator IAdaptor& () {
		return m_adaptor;
	}

	Adaptor(Adaptor&& other) : m_adaptor(other.m_adaptor) {
	}
	
private:
	Adaptor(const Adaptor& other) : m_adaptor(other.m_adaptor) {
	}

	Adaptor &operator=(const Adaptor& other) {
	}
};
