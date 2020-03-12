#pragma once

class Class;

class Object {
public:
	virtual Class &GetClass() const = 0;
	virtual ~Object() {
	}
};
