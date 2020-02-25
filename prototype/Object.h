#pragma once

class Class;

class Object {
public:
	virtual Class &GetClass() = 0;
	virtual ~Object() {
	}
};
