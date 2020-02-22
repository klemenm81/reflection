#pragma once

class Object;

class IInstantiator {
public:
	virtual Object& Instantiate() = 0;
	virtual ~IInstantiator() {
	}
};
