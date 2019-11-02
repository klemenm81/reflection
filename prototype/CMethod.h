#pragma once

#include "Adaptor.h"
#include "IMethod.h"

template <typename Class, typename Return, typename... Args>
class CMethodBase : public IMethod {
protected:
	template<typename Method, std::size_t... Index>
	IAdaptor& Invoke(Method method, IReflectable& obj, std::vector<IAdaptor*> args, std::index_sequence<Index...>) {
		if constexpr (std::is_same<Return, void>()) {
			(static_cast<Class&>(obj).*method)(
				(static_cast<CAdaptor<Args>&>(*args[Index])).GetValue()...
			);

			return *new CAdaptor<Return>();
		}
		else {
			return *new CAdaptor<Return>(
					(static_cast<Class&>(obj).*method)(
						(static_cast<CAdaptor<Args>&>(*args[Index])).GetValue()...
					)
				);
		}
	}
};

template <typename Class, typename Return, typename... Args>
class CMethod;

template <typename Class, typename Return, typename... Args>
class CMethod<Class, Return(Class::*)(Args...)> : public CMethodBase<Class, Return, Args...> {
private:
	Return(Class::* m_method)(Args...);

public:
	CMethod(Return(Class::* method)(Args...)) : m_method(method) {
	}

	IAdaptor& Invoke(IReflectable& obj, std::vector<IAdaptor*> args) {
		return CMethodBase<Class, Return, Args...>::Invoke(m_method, obj, args, std::index_sequence_for<Args...>{});
	}
};

template <typename Class, typename Return, typename... Args>
class CMethod<Class, Return(Class::*)(Args...), Args...> : public CMethodBase<Class, Return, Args...> {
private:
	Return(Class::* m_method)(Args...);

public:
	CMethod(Return(Class::* method)(Args...)) : m_method(method) {
	}

	IAdaptor& Invoke(IReflectable& obj, std::vector<IAdaptor*> args) {
		return CMethodBase<Class, Return, Args...>::Invoke(m_method, obj, args, std::index_sequence_for<Args...>{});
	}
};






template <typename Class, typename Return, typename... Args>
class CMethod<Class, Return(Class::* const)(Args...)> : public CMethodBase<Class, Return, Args...> {
private:
	Return(Class::* const m_method)(Args...);

public:
	CMethod(Return(Class::* const method)(Args...)) : m_method(method) {
	}

	IAdaptor& Invoke(IReflectable& obj, std::vector<IAdaptor*> args) {
		return CMethodBase<Class, Return, Args...>::Invoke(m_method, obj, args, std::index_sequence_for<Args...>{});
	}
};

template <typename Class, typename Return, typename... Args>
class CMethod<Class, Return(Class::* const)(Args...), Args...> : public CMethodBase<Class, Return, Args...> {
private:
	Return(Class::* const m_method)(Args...);

public:
	CMethod(Return(Class::* const method)(Args...)) : m_method(method) {
	}

	IAdaptor& Invoke(IReflectable& obj, std::vector<IAdaptor*> args) {
		return CMethodBase<Class, Return, Args...>::Invoke(m_method, obj, args, std::index_sequence_for<Args...>{});
	}
};







template <typename Class, typename Return, typename... Args>
class CMethod<Class, Return(Class::*)(Args...) const> : public CMethodBase<Class, Return, Args...> {
private:
	Return(Class::* m_method)(Args...) const;

public:
	CMethod(Return(Class::* method)(Args...) const) : m_method(method) {
	}

	IAdaptor& Invoke(IReflectable& obj, std::vector<IAdaptor*> args) {
		return CMethodBase<Class, Return, Args...>::Invoke(m_method, obj, args, std::index_sequence_for<Args...>{});
	}
};

template <typename Class, typename Return, typename... Args>
class CMethod<Class, Return(Class::*)(Args...) const, Args...> : public CMethodBase<Class, Return, Args...> {
private:
	Return(Class::* m_method)(Args...) const;

public:
	CMethod(Return(Class::* method)(Args...) const) : m_method(method) {
	}

	IAdaptor& Invoke(IReflectable& obj, std::vector<IAdaptor*> args) {
		return CMethodBase<Class, Return, Args...>::Invoke(m_method, obj, args, std::index_sequence_for<Args...>{});
	}
};







template <typename Class, typename Return, typename... Args>
class CMethod<Class, Return(Class::*)(Args...) noexcept> : public CMethodBase<Class, Return, Args...> {
private:
	Return(Class::* m_method)(Args...) noexcept;

public:
	CMethod(Return(Class::* method)(Args...) noexcept) : m_method(method) {
	}

	IAdaptor& Invoke(IReflectable& obj, std::vector<IAdaptor*> args) {
		return CMethodBase<Class, Return, Args...>::Invoke(m_method, obj, args, std::index_sequence_for<Args...>{});
	}
};







template <typename Class, typename Return, typename... Args>
class CMethod<Class, Return(Class::*)(Args...) const noexcept> : public CMethodBase<Class, Return, Args...> {
private:
	Return(Class::* m_method)(Args...) const noexcept;

public:
	CMethod(Return(Class::* method)(Args...) const noexcept) : m_method(method) {
	}

	IAdaptor& Invoke(IReflectable& obj, std::vector<IAdaptor*> args) {
		return CMethodBase<Class, Return, Args...>::Invoke(m_method, obj, args, std::index_sequence_for<Args...>{});
	}
};
