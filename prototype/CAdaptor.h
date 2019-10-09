#pragma once

#include "IAdaptor.h"

template <typename Type>
class CAdaptor : public IAdaptor {
private:
	Type m_value;

public:
	CAdaptor(Type value) : m_value(value) {
	}

	Type GetValue() {
		return m_value;
	}

	void SetValue(Type value) {
		m_value = value;
	}
};

template <>
class CAdaptor<void> : public IAdaptor {
public:
	CAdaptor() {
	}
};