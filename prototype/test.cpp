#include "test.h"

Test::Test() : a(5), myString("Reflection"), ptrString("Hello") {
	vec1.push_back(5);
	vec1.push_back(3);
}

Test::Test(int aa) : a(aa), myString("Reflection"), ptrString("Hello") {
	vec1.push_back(5);
	vec1.push_back(3);
}

Test::Test(const char *ptr) : a(1), myString(ptr), ptrString(ptr) {
	vec1.push_back(5);
	vec1.push_back(3);
}

Test::Test(int aa, std::vector<int> vec) : a(aa), vec1(vec) {
	vec1.push_back(5);
	vec1.push_back(3);
}

Test::Test(const Test& other) {

}

Test::Test(Test&& other) {

}


void Test::Foo1(std::string& str, float val) {
	str += std::to_string(val);
	printf("Foo(): str = %s\n", str.c_str());
}

int Test::Foo2(const char* str) {
	printf("Foo2(): str = %s\n", str);
	return 13;
}

int Test::FooOverloaded(int i) {
	return 100;
}

int Test::FooOverloaded(float i) {
	return 1;
}

int Test::FooOverloaded(std::vector<int> i, int j) {
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