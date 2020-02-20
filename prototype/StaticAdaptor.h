#pragma once

#include "CAdaptor.h"

class IAdaptor;

class StaticAdaptor {
private:
	IAdaptor &m_adaptor;

public:
	StaticAdaptor(IAdaptor& adaptor) : m_adaptor(adaptor) {
	}

	StaticAdaptor(IAdaptor&& adaptor) : m_adaptor(adaptor) {
	}
	
	StaticAdaptor(StaticAdaptor&& other) noexcept : m_adaptor(other.m_adaptor) {
	}

	StaticAdaptor& operator=(StaticAdaptor&& other) noexcept {
		m_adaptor = other.m_adaptor;
		return *this;
	}

	StaticAdaptor(const StaticAdaptor& other) : m_adaptor(other.m_adaptor) {
	}

	StaticAdaptor& operator=(const StaticAdaptor& other) {
		m_adaptor = other.m_adaptor;
		return *this;
	}

	template<typename Type>
	Type Get() const {
		return(static_cast<CAdaptor<Type>&>(m_adaptor).GetValue());
	}

	template<typename Type>
	void Set(Type value) {
		static_cast<CAdaptor<Type>&>(m_adaptor).SetValue(value);
	}

	operator IAdaptor& () const {
		return m_adaptor;
	}
};
