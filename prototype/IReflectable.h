#pragma once

class IClass;

class IReflectable {
public:
	virtual IClass& GetClass() = 0;
	virtual ~IReflectable() {
	}
};
