#include "test.h"

Test::Test() : a(5), myString("Reflection"), ptrString(L"Hello") {
}

void Test::Foo1(std::wstring& str, float val) {
	str += std::to_wstring(val);
	wprintf(L"Foo(): str = %s\n", str.c_str());
}

int Test::Foo2(const wchar_t* str) {
	wprintf(L"Foo2(): str = %s\n", str);
	return 13;
}

int Test::FooOverloaded(int i) {
	return 0;
}

int Test::FooOverloaded(float i) {
	return 1;
}

int Test::FooOverloaded(std::vector<int> i) const {
	return -1;
}

std::string Test::FooConst() const {
	return "From FooConst()";
}


void Test::Bar1() {
}

void Test::Bar2() const {
}

void Test::Bar3() noexcept {
}

void Test::Bar4() const noexcept {
}

void Test::Bar5() volatile {
}

void Test::Bar6() volatile const {
}

void Test::Bar7() volatile noexcept{
}

void Test::Bar8() volatile const noexcept {
}

void Test::Bar9() & {
}

void Test::Bar10() const & {
}

void Test::Bar11() & noexcept {
}

void Test::Bar12() const & noexcept {
}

void Test::Bar13() volatile & {
}

void Test::Bar14() volatile const & {
}

void Test::Bar15() volatile & noexcept {
}

void Test::Bar16() volatile const & noexcept {
}

void Test::Bar17() & {
	printf("From Bar17\n");
}

void Test::Bar17() && {
	printf("From Bar17 &&\n");
}

void Test::Bar18() const && {
}

void Test::Bar19() && noexcept {
}

void Test::Bar20() const && noexcept {
}

void Test::Bar21() volatile && {
}

void Test::Bar22() volatile const && {
}

void Test::Bar23() volatile && noexcept {
}

void Test::Bar24() volatile const && noexcept {
}

int TestDerived::FooOverloaded(int i) {
	return 0;
}