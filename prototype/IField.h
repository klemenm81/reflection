#pragma once

class IAdaptor;
class Object;

class IField
{
public:
	virtual IAdaptor& GetValue(const Object& obj) = 0;
	virtual void SetValue(Object& obj, IAdaptor& value) = 0;
	virtual ~IField() {
	}
};
