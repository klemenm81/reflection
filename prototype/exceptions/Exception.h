#pragma once

class Exception {
public:
	virtual const char *GetMessage() const = 0;
};