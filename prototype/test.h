#pragma once

#include "Reflectable.h"
#include <vector>

class Test : public Reflectable<Test> {
	REFLECT_FULL_ACCESS;		// enables access to private and protected members via reflection

private:
	int a;
	std::string myString;
	const char* ptrString;

public:
	Test();
	void Foo1(std::string& str, float val);
	int Foo2(const char* str);
	int FooOverloaded(int i);
	int FooOverloaded(float i);
	int FooOverloaded(std::vector<int> i) const;
	std::string FooConst() const;

	void Fcn1() {
	}

	void Fcn2() const {
	}

	void Fcn3() volatile {
	}

	void Fcn4() const volatile {
	}

	void Fcn5() noexcept {
	}

	void Fcn6() const noexcept {
	}

	void Fcn7() volatile noexcept {
	}

	void Fcn8() const volatile noexcept{
	}


	void Fcn9() & {
	}

	void Fcn10() const & {
	}

	void Fcn11() volatile & {
	}

	void Fcn12() const volatile & {
	}

	void Fcn13() & noexcept {
	}

	void Fcn14() const & noexcept {
	}

	void Fcn15() volatile & noexcept {
	}

	void Fcn16() const volatile & noexcept {
	}


	void Fcn17()&& {
	}

	void Fcn18() const&& {
	}

	void Fcn19() volatile&& {
	}

	void Fcn20() const volatile&& {
	}

	void Fcn21() && noexcept {
	}

	void Fcn22() const&& noexcept {
	}

	void Fcn23() volatile&& noexcept {
	}

	void Fcn24() const volatile&& noexcept {
	}





	void Bar1();
	void Bar2() const;
	void Bar3() noexcept;
	void Bar4() const noexcept;
	void Bar5() volatile;
	void Bar6() volatile const;
	void Bar7() volatile noexcept;
	void Bar8() volatile const noexcept;

	void Bar9() &;
	void Bar10() const &;
	void Bar11() & noexcept;
	void Bar12() const & noexcept;
	void Bar13() volatile &;
	void Bar14() volatile const &;
	void Bar15() volatile & noexcept;
	void Bar16() volatile const & noexcept;

	void Bar17() &&;
	void Bar17() &;
	void Bar18() const &&;
	void Bar19() && noexcept;
	void Bar20() const && noexcept;
	void Bar21() volatile &&;
	void Bar22() volatile const &&;
	void Bar23() volatile && noexcept;
	void Bar24() volatile const && noexcept;

	
	void Overload(int) 
	{
	}
	void Overload(int) const 
	{
	}
	
	void Overload(int) const volatile 
	{
	}
	void Overload(int) volatile
	{
	}
	

	void OverloadEx(int) & 
	{
	}
	void OverloadEx(int) const &
	{
	}
	void OverloadEx(int) const volatile &
	{
	}
	void OverloadEx(int) volatile &
	{
	}


	void OverloadEx(int)&&
	{
	}
	void OverloadEx(int) const&&
	{
	}
	void OverloadEx(int) const volatile&&
	{
	}
	void OverloadEx(int) volatile&&
	{
	}


};

class TestDerived : public Test, public Reflectable<TestDerived> {
public:
	int FooOverloaded(int i);
};