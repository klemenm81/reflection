#include "Reflectable.h"
#include "CClass.h"
#include "CField.h"
#include "CMethod2.h"
#include "CAdaptor.h"
#include "Adaptor.h"

#include "IMethod.h"

#include <vector>

class Test : public Reflectable<Test> {
	friend class CClass<Test>;		// if we want to have access to private and protected members via reflection, we need this friend

private:
	int a;
	std::string myString;
	const wchar_t* ptrString;

public:
	Test() : a(5), myString("Reflection"), ptrString(L"Hello") {
	}

	void Foo(std::wstring& str, float val) {
		str += std::to_wstring(val);
	}
};


int main() {
	Test test;
	IField& field1 = test.GetReflection().GetField("a");
	IField& field2 = test.GetReflection().GetField("myString");
	IField& field3 = test.GetReflection().GetField("ptrString");
	IMethod& method = test.GetReflection().GetMethod("Foo");

	Adaptor adaptor1 = field1.GetValue(test);
	Adaptor adaptor2 = field2.GetValue(test);
	Adaptor adaptor3 = field3.GetValue(test);

	int see1 = adaptor1.Get<int>();
	std::string see2 = adaptor2.Get<std::string>();
	const wchar_t *see3 = adaptor3.Get<const wchar_t *>();

	std::wstring str = L"PI = ";
	std::vector<IAdaptor*> args;
	args.push_back(new CAdaptor<std::wstring&>(str));
	args.push_back(new CAdaptor<float>(3.14f));

	Adaptor retValAdaptor = method.Invoke(test, args);

	adaptor1.Set(13);
	field1.SetValue(test, adaptor1);
}
/*
std::map<std::string, IField*> CClass<Test>::m_fields;
std::map<std::string, IMethod*> CClass<Test>::m_methods;
template<>
CClass<Test>::CClass() {
	m_fields["a"] = new CField<Test, int>(&Test::a);
	m_fields["myString"] = new CField<Test, std::string>(&Test::myString);
	m_fields["ptrString"] = new CField<Test, const wchar_t *>(&Test::ptrString);
	m_methods["Foo"] = new CMethod2<Test, std::wstring&, float>(&Test::Foo);
}
*/
DEFINE_CLASS_START(Test)
	DEFINE_METHOD(Foo, std::wstring&, float)
	DEFINE_FIELD(int, a)
	DEFINE_FIELD(std::string, myString)
	DEFINE_FIELD(const wchar_t *, ptrString)
DEFINE_CLASS_END