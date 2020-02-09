#pragma once

class IAdaptor;
class IReflectable;

class IField
{
public:
	virtual IAdaptor& GetValue(std::byte* retValBuffer, IAdaptor& obj) = 0;
	virtual void SetValue(IAdaptor& obj, IAdaptor& value) = 0;
	virtual ~IField() {
	}
};
