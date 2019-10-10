#pragma once

class IAdaptor;
class IReflectable;

class IField
{
public:
	virtual IAdaptor &GetValue(IReflectable& obj) = 0;
	virtual void SetValue(IReflectable& obj, IAdaptor &value) = 0;
	virtual ~IField() {
	}
};
