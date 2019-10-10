#pragma once

class IReflectable;

class IInstantiator {
public:
	virtual IReflectable& Instantiate() = 0;
	virtual ~IInstantiator() {
	}
};
