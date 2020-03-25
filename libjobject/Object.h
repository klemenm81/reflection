#pragma once

class Class;

class Object {
public:
	virtual Class &getClass() const = 0;
	virtual ~Object() {
	}
};
