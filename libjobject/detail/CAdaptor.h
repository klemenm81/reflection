#pragma once


#include "IAdaptor.h"
#include "../Serialization.h"
#include "../exceptions/MarshallingException.h"
#include "../TypeInfo.h"
#include <optional>
#include <string>
#include <vector>
#include <forward_list>

template <typename Type>
class CAdaptor : public IAdaptor {
private:
	Type m_value;

public: 
	static Indirection GetIndirection() {
		return Value;
	}

	static const char* GetSignature() {
		static const std::string signature = std::to_string(TypeInfo<Type>::getUniqueId());
		return signature.c_str();
	}

	static const char* GetName() {
		static const std::string name = TypeInfo<Type>::getName();
		return name.c_str();
	}

	static bool IsOptional() {
		return false;
	}

	CAdaptor(const Type &value) : m_value(value) {
	}
	
	CAdaptor(Type&& value) : m_value(std::move(value)) {
	}

	CAdaptor(Json::Value value) : m_value(Serialization<Type>::Deserialize(value)) {
	}

	Indirection getIndirection() const {
		return GetIndirection();
	}
	
	const Type &getValue() {
		return m_value;
	}
	
	const char* getSignature() const {
		return GetSignature();
	}

	const char* getName() const {
		return GetName();
	}
	
	Json::Value marshall() const {
		return Serialization<Type>::Serialize(m_value);
	}
};

template <typename Type>
class CAdaptor<Type*> : public IAdaptor {
private:
	Type* m_value;

public:
	static Indirection GetIndirection() {
		return Pointer;
	}

	static const char* GetSignature() {
		static const std::string signature = std::to_string(TypeInfo<Type*>::getUniqueId());
		return signature.c_str();
	}

	static const char* GetName() {
		static const std::string name = TypeInfo<Type*>::getName();
		return name.c_str();
	}

	static bool IsOptional() {
		return false;
	}

	CAdaptor(Type *value) : m_value(value) {
	}

	CAdaptor(Json::Value value) {
		throw MarshallingException("Pointers cannot be unmarshalled");;
	}

	Indirection getIndirection() const {
		return GetIndirection();
	}

	Type *getValue() {
		return m_value;
	}
	
	const char* getSignature() const {
		return GetSignature();
	}

	const char* getName() const {
		return GetName();
	}
	
	Json::Value marshall() const {
		throw MarshallingException("Pointers cannot be marshalled");
	}
};


template <typename Type>
class CAdaptor<Type&> : public IAdaptor {
private:
	Type *m_value;

public:
	static Indirection GetIndirection() {
		return LValueReference;
	}

	static const char* GetSignature() {
		static const std::string signature = std::to_string(TypeInfo<Type&>::getUniqueId());
		return signature.c_str();
	}

	static const char* GetName() {
		static const std::string name = TypeInfo<Type&>::getName();
		return name.c_str();
	}

	static bool IsOptional() {
		return false;
	}

	CAdaptor(Type &value) : m_value(&value) {
	}

	CAdaptor(Json::Value value) {
		throw MarshallingException("LValue references cannot be unmarshalled");;
	}

	Indirection getIndirection() const {
		return GetIndirection();
	}

	Type& getValue() {
		return *m_value;
	}
	
	const char* getSignature() const {
		return GetSignature();
	}

	const char* getName() const {
		return GetName();
	}
	
	Json::Value marshall() const {
		throw MarshallingException("LValue references cannot be marshalled");
	}
};

template <typename Type>
class CAdaptor<Type &&> : public IAdaptor {
private:
	Type &&m_value;

public:
	static Indirection GetIndirection() {
		return RValueReference;
	}

	static const char* GetSignature() {
		static const std::string signature = std::to_string(TypeInfo<Type&&>::getUniqueId());
		return signature.c_str();
	}

	static const char* GetName() {
		static const std::string name = TypeInfo<Type&&>::getName();
		return name.c_str();
	}

	static bool IsOptional() {
		return false;
	}

	CAdaptor(Type &&value) : m_value(std::forward<Type>(value)) {
	}

	CAdaptor(Json::Value value) {
		MarshallingException("RValue references cannot be unmarshalled");
	}

	Indirection getIndirection() const {
		return GetIndirection();
	}

	Type&& getValue() {
		return std::forward<Type>(m_value);
	}
	
	const char* getSignature() const {
		return GetSignature();
	}

	const char* getName() const {
		return GetName();
	}
	
	Json::Value marshall() const {
		throw MarshallingException("RValue references cannot be marshalled");
	}
};

template <typename Type>
class CAdaptor<std::optional<Type>> : public IAdaptor {
private:
	std::optional<Type> m_value;

public:
	static Indirection GetIndirection() {
		return Value;
	}

	static const char* GetSignature() {
		static const std::string signature = std::to_string(TypeInfo<std::optional<Type>>::getUniqueId());
		return signature.c_str();
	}

	static const char* GetName() {
		static const std::string name = TypeInfo<std::optional<Type>>::getName();
		return name.c_str();
	}

	static bool IsOptional() {
		return true;
	}

	CAdaptor(const std::optional<Type>& value) : m_value(value) {
	}

	CAdaptor(std::optional<Type>&& value) : m_value(std::move(value)) {
	}

	CAdaptor(Json::Value value) : m_value(Serialization<std::optional<Type>>::Deserialize(value)) {
	}

	Indirection getIndirection() const {
		return GetIndirection();
	}

	const std::optional<Type>& getValue() {
		return m_value;
	}

	const char* getSignature() const {
		return GetSignature();
	}

	const char* getName() const {
		return GetName();
	}

	Json::Value marshall() const {
		return Serialization<std::optional<Type>>::Serialize(m_value);
	}
};


template <>
class CAdaptor<void> : public IAdaptor {
public:
	static Indirection GetIndirection() {
		return Value;
	}

	static const char* GetSignature() {
		static const std::string signature = std::to_string(TypeInfo<void>::getUniqueId());
		return signature.c_str();
	}

	static const char* GetName() {
		static const std::string name = TypeInfo<void>::getName();
		return name.c_str();
	}

	static bool IsOptional() {
		return false;
	}

	CAdaptor() {
	}

	CAdaptor(Json::Value value) {
	}

	Indirection getIndirection() const {
		return GetIndirection();
	}
	
	const char* getSignature() const {
		return GetSignature();
	}

	const char* getName() const {
		return GetName();
	}
	
	Json::Value marshall() const {
		return Json::Value("");
	}
};