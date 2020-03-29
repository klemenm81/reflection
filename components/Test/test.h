#pragma once

#include "../../libjobject/Reflectable.h"
#include <vector>

class Test : public Reflectable<Test> {
	REFLECT_FULL_ACCESS;		// enables access to private and protected members via reflection

private:
	int a;
	std::string myString;
	const char* ptrString;
	std::vector<int> vec1;

public:
	Test();
	Test(int aa);
	Test(const char *ptr);
	Test(int aa, std::vector<int> vec);

	Test(const Test& other);
	Test(Test&& other);

	void foo1(std::string& str, float val);
	int foo2(const char* str);
	int fooOverloaded(int i);
	int fooOverloaded(float i);
	int fooOverloaded(std::vector<int> i, int j);
	std::string fooConst() const;

	void fcn1() {
	}

	void fcn2() const {
	}

	void fcn3() volatile {
	}

	void fcn4() const volatile {
	}

	void fcn5() noexcept {
	}

	void fcn6() const noexcept {
	}

	void fcn7() volatile noexcept {
	}

	void fcn8() const volatile noexcept{
	}


	void fcn9() & {
	}

	void fcn10() const & {
	}

	void fcn11() volatile & {
	}

	void fcn12() const volatile & {
	}

	void fcn13() & noexcept {
	}

	void fcn14() const & noexcept {
	}

	void fcn15() volatile & noexcept {
	}

	void fcn16() const volatile & noexcept {
	}


	void fcn17()&& {
	}

	void fcn18() const&& {
	}

	void fcn19() volatile&& {
	}

	void fcn20() const volatile&& {
	}

	void fcn21() && noexcept {
	}

	void fcn22() const&& noexcept {
	}

	void fcn23() volatile&& noexcept {
	}

	void fcn24() const volatile&& noexcept {
	}





	void bar1();
	void bar2() const;
	void bar3() noexcept;
	void bar4() const noexcept;
	void bar5() volatile;
	void bar6() volatile const;
	void bar7() volatile noexcept;
	void bar8() volatile const noexcept;

	void bar9() &;
	void bar10() const &;
	void bar11() & noexcept;
	void bar12() const & noexcept;
	void bar13() volatile &;
	void bar14() volatile const &;
	void bar15() volatile & noexcept;
	void bar16() volatile const & noexcept;

	void bar17() &&;
	void bar17() &;
	void bar18() const &&;
	void bar19() && noexcept;
	void bar20() const && noexcept;
	void bar21() volatile &&;
	void bar22() volatile const &&;
	void bar23() volatile && noexcept;
	void bar24() volatile const && noexcept;

	
	void overload(int) 
	{
	}
	void overload(int) const 
	{
	}
	
	void overload(int) const volatile 
	{
	}
	void overload(int) volatile
	{
	}
	

	void overloadEx(int) & 
	{
	}
	void overloadEx(int) const &
	{
	}
	void overloadEx(int) const volatile &
	{
	}
	void overloadEx(int) volatile &
	{
	}


	void overloadEx(int)&&
	{
	}
	void overloadEx(int) const&&
	{
	}
	void overloadEx(int) const volatile&&
	{
	}
	void overloadEx(int) volatile&&
	{
	}


};

class TestDerived : public Test, public Reflectable<TestDerived> {
public:
	int fooOverloaded(int i);
	int fooMarshal1(Test test);
};