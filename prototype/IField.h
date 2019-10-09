#pragma once

#include "IAdaptor.h"
#include "IReflectable.h"

class IField
{
public:
	virtual IAdaptor &GetValue(IReflectable& obj) = 0;
	virtual void SetValue(IReflectable& obj, IAdaptor &value) = 0;
	virtual ~IField() {
	}
};
