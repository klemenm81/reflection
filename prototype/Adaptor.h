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

	Adaptor(Adaptor&& other) noexcept : m_adaptor(other.m_adaptor) {
		other.m_adaptor = nullptr;
	}

	Adaptor& operator=(Adaptor&& other) noexcept {
		m_adaptor = other.m_adaptor;
		other.m_adaptor = nullptr;
		return *this;
	}

	Adaptor(const Adaptor& other) = delete;
	Adaptor& operator=(const Adaptor& other) = delete;

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
};
