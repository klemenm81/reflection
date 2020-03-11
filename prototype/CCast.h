#pragma once

#include "CAdaptor.h"
#include "Object.h"
#include "Reflectable.h"
#include "ICast.h"

template <typename Class, typename Cast>
class CCast : public ICast {
private:
	std::string m_signature;
	std::string m_name;

public:
	CCast(const char* signature, const char* name) : m_signature(signature), m_name(name) {
	}

	const char* GetSignature() {
		return m_signature.c_str();
	}

	const char* GetName() {
		return m_name.c_str();
	}

	IAdaptor* CastClass(Object& obj) {
		static thread_local std::byte retValBuffer[sizeof(CAdaptor<Cast&>)];
		Class& clasz = static_cast<Class&>(static_cast<Reflectable<Class>&>(obj));
		return new(retValBuffer) CAdaptor<Cast&>(static_cast<Cast&>(clasz));
	}
};