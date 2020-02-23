#pragma once

class Exception {
public:
	virtual const char *Message() const = 0;
};