#pragma once

#include "CAdaptor.h"

class IAdaptor;

class DynamicAdaptor {
private:
	IAdaptor* m_adaptor;

public:
	DynamicAdaptor() : m_adaptor(nullptr) {
	}

	DynamicAdaptor(IAdaptor *adaptor) : m_adaptor(adaptor) {
	}

	DynamicAdaptor(DynamicAdaptor&& other) noexcept : m_adaptor(other.m_adaptor) {
		other.m_adaptor = nullptr;
	}

	DynamicAdaptor& operator=(DynamicAdaptor&& other) noexcept {
		m_adaptor = other.m_adaptor;
		other.m_adaptor = nullptr;
		return *this;
	}

	DynamicAdaptor(const DynamicAdaptor& other) = delete;
	DynamicAdaptor& operator=(const DynamicAdaptor& other) = delete; 

	template<typename Type>
	Type Get() const {
		return(static_cast<CAdaptor<Type>&>(*m_adaptor).GetValue());
	}

	template<typename Type>
	void Set(Type value) {
		static_cast<CAdaptor<Type>&>(*m_adaptor).SetValue(value);
	}

	operator IAdaptor& () const {
		return *m_adaptor;
	}

	IAdaptor* GetPtr() const {
		return m_adaptor;
	}

	~DynamicAdaptor() {
		if (m_adaptor) {
			delete m_adaptor;
		}
	}
};
