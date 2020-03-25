#pragma once

#include "IAdaptor.h"
#include "../Object.h"
#include "../../libjson/include/json/json.h"

class IField
{
public:
	virtual const char* getName() const = 0;

	virtual IAdaptor& getValue(const Object& obj) const = 0;
	virtual void setValue(Object& obj, IAdaptor& value) const = 0;

	virtual Json::Value serialize(const Object& obj) const = 0;
	virtual void deserialize(Object& obj, Json::Value value) const = 0;

	virtual const char* getTypeSignature() const = 0;
	virtual const char* getTypeName() const = 0;

	virtual ~IField() {
	}
};
