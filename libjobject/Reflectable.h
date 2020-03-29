#pragma once

#include "detail/CClass.h"
#include "Object.h"
#include "Class.h"

#define REFLECT_FULL_ACCESS											\
	friend void CClass<ReflectedClass>::registerMetadata<ReflectedClass>();

#define REFLECT_PUBLIC_ACCESS

template <typename ClassType>
class Reflectable : public Object {
protected:
	typedef ClassType ReflectedClass;
public:
	Class &getClass() const {
		static CClass<ClassType> reflection;
		static Class clasz(reflection);
		return clasz;
	}

	Json::Value serialize() const {
		Json::Value ret;
		std::vector<Field> fields = getClass().getFields();
		for (Field field : fields) {
			ret[field.getName()] = field.serialize(*this);
		}
		return ret;
	}

	void deserialize(Json::Value value) {
		Json::Value::Members members = value.getMemberNames();
		for (std::string memberName : members) {
			Field field = getClass().getField(memberName.c_str());
			field.deserialize(*this, value[memberName].asString());
		}
	}
};
