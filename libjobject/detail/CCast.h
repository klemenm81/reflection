#pragma once

#include "CAdaptor.h"
#include "ICast.h"
#include "../Object.h"
#include "../Reflectable.h"

template <typename Class, typename Cast>
class CCast : public ICast {
private:
	std::string m_signature;
	std::string m_name;

public:
	CCast(const char* signature, const char* name) : m_signature(signature), m_name(name) {
	}

	const char* getSignature() const {
		return m_signature.c_str();
	}

	const char* getName() const {
		return m_name.c_str();
	}

	IAdaptor* castClass(Object& obj) const {
		static thread_local std::byte retValBuffer[sizeof(CAdaptor<Cast&>)];
		Class& clasz = static_cast<Class&>(static_cast<Reflectable<Class>&>(obj));
		return new(retValBuffer) CAdaptor<Cast&>(static_cast<Cast&>(clasz));
	}
};