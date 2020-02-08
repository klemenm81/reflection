#pragma once

#include "CAdaptor.h"

class IAdaptor;

class Adaptor2 {
private:
	IAdaptor&& m_adaptor;

public:
	Adaptor2(IAdaptor&& adaptor) : m_adaptor(std::move(adaptor)) {
	}

	Adaptor2(Adaptor2&& other) noexcept : m_adaptor(std::move(other.m_adaptor)) {
	}

	Adaptor2& operator=(Adaptor2&& other) noexcept {
		m_adaptor = std::move(other.m_adaptor);
		return *this;
	}

	Adaptor2(const Adaptor2& other) = delete;
	Adaptor2& operator=(const Adaptor2& other) = delete;

	~Adaptor2() {
	}

	template<typename Type>
	Type Get() {
		return(static_cast<CAdaptor<Type>&>(m_adaptor).GetValue());
	}

	template<typename Type>
	void Set(Type value) {
		static_cast<CAdaptor<Type>&>(m_adaptor).SetValue(value);
	}
};
