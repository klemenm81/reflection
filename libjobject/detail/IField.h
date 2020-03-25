#pragma once

#include "IAdaptor.h"
#include "../Object.h"
#include "../../libjson/include/json/json.h"

class IField
{
public:
	virtual const char* GetName() const = 0;

	virtual IAdaptor& getValue(const Object& obj) const = 0;
	virtual void SetValue(Object& obj, IAdaptor& value) const = 0;

	virtual Json::Value Serialize(const Object& obj) const = 0;
	virtual void Deserialize(Object& obj, Json::Value value) const = 0;

	virtual const char* GetTypeSignature() const = 0;
	virtual const char* GetTypeName() const = 0;

	virtual ~IField() {
	}
};
