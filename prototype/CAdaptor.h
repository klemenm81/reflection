#pragma once
#include <forward_list>
#include "IAdaptor.h"

template <typename Type>
class CAdaptor : public IAdaptor {
private:
	Type m_value;

public:
	CAdaptor() {
	}

	CAdaptor(Type value) : m_value(value) {
	}

	Type GetValue() {
		return m_value;
	}

	void SetValue(Type value) {
		m_value = value;
	}
};

template <typename Type>
class CAdaptor<Type&> : public IAdaptor {
private:
	Type *m_value;

public:
	CAdaptor() {
	}

	CAdaptor(Type &value) : m_value(&value) {
	}

	Type& GetValue() {
		return *m_value;
	}

	void SetValue(Type& value) {
		m_value = &value;
	}
};

template <typename Type>
class CAdaptor<Type &&> : public IAdaptor {
private:
	Type *m_value;

public:
	CAdaptor() {
	}

	CAdaptor(Type && value) : m_value(&std::forward<Type>(value)) {
	}

	Type && GetValue() {
		return std::forward<Type>(*m_value);
	}

	void SetValue(Type && value) {
		m_value = &value;
	}
};

template <>
class CAdaptor<void> : public IAdaptor {
public:
	CAdaptor() {
	}
};