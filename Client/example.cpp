#include "example.h"

#include "Metadata.h"

#include <stdexcept>
	
class ExampleClass : public Reflectable<ExampleClass> {
    REFLECT_FULL_ACCESS;
private:
    float m_factor;
    ExampleStruct m_struct;

public:
    ExampleClass() : m_factor(1) {
        m_struct.mainValue = 0;
    }
    ExampleClass(std::string key, std::vector<int> values)
        : m_factor(1) {
        m_struct.mainValue = 0;
        m_struct.values[key] = values;
    }
    ExampleClass(const ExampleClass& other)
        : m_factor(other.m_factor), m_struct(other.m_struct) {
    }
    void clear() {
        m_factor = 1;
        m_struct.values.clear();
    }
    void setFactor(float factor) {
        m_factor = factor;
    }
    void add(std::string key, std::vector<int> values) {
        m_struct.values[key] = values;
    }
    void add(const ExampleStruct& exampleStruct) {
        for (auto it = exampleStruct.values.begin();
            it != exampleStruct.values.end();
            it++) {
            m_struct.values[it->first] = it->second;
        }
        m_struct.mainValue += exampleStruct.mainValue;
    }
    std::vector<float> getValues(const char *key) const {
        auto it = m_struct.values.find(key);
        if (it != m_struct.values.end()) {
            std::vector<float> ret;
            for (int val : it->second) ret.push_back(m_factor * val);
            return ret;
        }
        throw std::runtime_error("Not found!");
    }
    std::vector<float> getValues(const char* key) {
        std::vector<float> ret;
        std::vector<int> vec = m_struct.values[key];
        for (int val : vec) ret.push_back(val * m_factor);
        return ret;
    }
    double testPerformance(std::string key, std::vector<int> values) {
        return 0.0;
    }
};

REFLECT_TYPE_START(ExampleStruct)
	FIELD(mainValue)
	FIELD(values)
REFLECT_TYPE_END

REFLECT_TYPE_START(ExampleClass)
	CONSTRUCTOR(ExampleClass, std::string, std::vector<int>)
    FIELD(m_factor)
    FIELD(m_struct)
	METHOD(clear)
    METHOD(setFactor)
	METHOD(void, add, std::string, std::vector<int>)
	METHOD(void, add, const ExampleStruct &)
    METHOD(std::vector<float>, getValues, const char* key)
    METHOD(testPerformance)
REFLECT_TYPE_END
