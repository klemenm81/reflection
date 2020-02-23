#pragma once

#include "Class.h"

class Object {
public:
	virtual Class GetClass() = 0;
	virtual ~Object() {
	}
};
