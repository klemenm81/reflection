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

	const char* toString() const {
		static thread_local std::string ret;
		Json::Value json = serialize();
		Json::StreamWriterBuilder wbuilder;
		wbuilder["indentation"] = "\t";
		ret = Json::writeString(wbuilder, json);
		return ret.c_str();
	}

	void deserialize(Json::Value value) {
		Json::Value::Members members = value.getMemberNames();
		for (std::string memberName : members) {
			Field field = getClass().getField(memberName.c_str());
			field.deserialize(*this, value[memberName]);
		}
	}

	void fromString(const char* val) {
		Json::CharReaderBuilder rbuilder;
		rbuilder["collectComments"] = false;
		std::string errs;
		std::stringstream s(val);
		Json::Value json;
		bool result = Json::parseFromStream(rbuilder, s, &json, &errs);
		if (!result) {
			std::stringstream s(std::string("\"") + val + std::string("\""));
			bool result = Json::parseFromStream(rbuilder, s, &json, &errs);
		}
		deserialize(json);
	}
};
