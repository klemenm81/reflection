#pragma once

class IAdaptor {
public:
	virtual const char* GetSignature() = 0;
	virtual const char* GetName() = 0;
	virtual ~IAdaptor() {
	}
};
