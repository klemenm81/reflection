#include "metadata.h"
#include "test.h"

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