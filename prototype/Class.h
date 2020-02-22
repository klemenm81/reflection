#pragma once

#include "IClass.h"
#include "Method.h"
#include "Field.h"

class Class {
private:
	IClass& m_class;

public:
	Class(IClass& clasz) : m_class(clasz) {
	}

	Class(Class&& other) noexcept : m_class(other.m_class) {
	}

	Class& operator=(Class&& other) noexcept {
		m_class = other.m_class;
	}

	Class(const Class& other) : m_class(other.m_class) {
	}

	Class& operator=(const Class& other) {
		m_class = other.m_class;
	}

	Field GetField(const char *name) {
		return m_class.GetField(name);
	}

	Method GetMethod(const char *name) {
		return m_class.GetMethod(name);
	}
};
