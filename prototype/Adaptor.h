#pragma once

#include "CAdaptor.h"

class IAdaptor;

class Adaptor {
private:
	IAdaptor *m_adaptor;

public:
	Adaptor() : m_adaptor(nullptr) {
	}

	Adaptor(IAdaptor &adaptor) : m_adaptor(&adaptor) {
	}

	~Adaptor() {
		if (m_adaptor != nullptr) {
			delete m_adaptor;
		}
	}

	template<typename Type>
	Type Get() {
		return(static_cast<CAdaptor<Type>&>(*m_adaptor).GetValue());
	}

	template<typename Type>
	void Set(Type value) {
		static_cast<CAdaptor<Type>&>(*m_adaptor).SetValue(value);
	}

	operator IAdaptor& () {
		return *m_adaptor;
	}

	Adaptor(Adaptor&& other) noexcept : m_adaptor(other.m_adaptor) {
	}

	Adaptor& operator=(IAdaptor& other) {
		if (m_adaptor != nullptr) {
			delete m_adaptor;
		}
		m_adaptor = &other;
		return *this;
	}
	
private:
	Adaptor(const Adaptor& other) : m_adaptor(other.m_adaptor) {
	}

	Adaptor &operator=(const Adaptor& other) {
		return *this;
	}
};
