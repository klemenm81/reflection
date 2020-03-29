#pragma once

#include "../libjson/include/json/json.h"

class Class;

class Object {
public:
	virtual Class &getClass() const = 0;
	virtual Json::Value serialize() const = 0;
	virtual void deserialize(Json::Value) = 0;
	virtual ~Object() {
	}
};
