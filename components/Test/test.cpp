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


void Test::foo1(std::string& str, float val) {
	str += std::to_string(val);
	printf("Foo(): str = %s\n", str.c_str());
}

int Test::foo2(const char* str) {
	printf("Foo2(): str = %s\n", str);
	return 13;
}

int Test::fooOverloaded(int i) {
	return 100;
}

int Test::fooOverloaded(float i) {
	return 1;
}

int Test::fooOverloaded(std::vector<int> i, int j) {
	return -1;
}

std::string Test::fooConst() const {
	return "From fooConst()";
}

void Test::bar1() {
}

void Test::bar2() const {
}

void Test::bar3() noexcept {
}

void Test::bar4() const noexcept {
}

void Test::bar5() volatile {
}

void Test::bar6() volatile const {
}

void Test::bar7() volatile noexcept{
}

void Test::bar8() volatile const noexcept {
}

void Test::bar9() & {
}

void Test::bar10() const & {
}

void Test::bar11() & noexcept {
}

void Test::bar12() const & noexcept {
}

void Test::bar13() volatile & {
}

void Test::bar14() volatile const & {
}

void Test::bar15() volatile & noexcept {
}

void Test::bar16() volatile const & noexcept {
}

void Test::bar17() & {
	printf("From Bar17\n");
}

void Test::bar17() && {
	printf("From Bar17 &&\n");
}

void Test::bar18() const && {
}

void Test::bar19() && noexcept {
}

void Test::bar20() const && noexcept {
}

void Test::bar21() volatile && {
}

void Test::bar22() volatile const && {
}

void Test::bar23() volatile && noexcept {
}

void Test::bar24() volatile const && noexcept {
}

int TestDerived::fooOverloaded(int i) {
	return 0;
}