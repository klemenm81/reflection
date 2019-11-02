#pragma once

#include "Adaptor.h"
#include "IMethod.h"

template <typename Class, typename Return, typename... Args>
class CMethodBase : public IMethod {
protected:
	template<typename Method, std::size_t... Index>
	IAdaptor& Invoke(Method method, IAdaptor& adaptor, std::vector<IAdaptor*> args, std::index_sequence<Index...>) {
		if constexpr (std::is_same<Return, void>()) {
			(static_cast<CAdaptor<Class&>&>(adaptor).GetValue().*method)(
				(static_cast<CAdaptor<Args>&>(*args[Index])).GetValue()...
			);
			return *new CAdaptor<Return>();
		}
		else {
			return *new CAdaptor<Return>(
				(static_cast<CAdaptor<Class&>&>(adaptor).GetValue().*method)(
					(static_cast<CAdaptor<Args>&>(*args[Index])).GetValue()...
				)
			);
		}
	}

	template<typename Method, std::size_t... Index>
	IAdaptor& InvokeRValue(Method method, IAdaptor& adaptor, std::vector<IAdaptor*> args, std::index_sequence<Index...>) {
		if constexpr (std::is_same<Return, void>()) {
			(static_cast<CAdaptor<Class&&>&>(adaptor).GetValue().*method)(
				(static_cast<CAdaptor<Args>&>(*args[Index])).GetValue()...
			);
			return *new CAdaptor<Return>();
		}
		else {
			return *new CAdaptor<Return>(
				(static_cast<CAdaptor<Class&&>&>(adaptor).GetValue().*method)(
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

	IAdaptor& Invoke(IAdaptor& adaptor, std::vector<IAdaptor*> args) {
		return CMethodBase<Class, Return, Args...>::Invoke(m_method, adaptor, args, std::index_sequence_for<Args...>{});
	}
};

template <typename Class, typename Return, typename... Args>
class CMethod<Class, Return(Class::*)(Args...) const> : public CMethodBase<Class, Return, Args...> {
private:
	Return(Class::* m_method)(Args...) const;

public:
	CMethod(Return(Class::* method)(Args...) const) : m_method(method) {
	}

	IAdaptor& Invoke(IAdaptor& adaptor, std::vector<IAdaptor*> args) {
		return CMethodBase<Class, Return, Args...>::Invoke(m_method, adaptor, args, std::index_sequence_for<Args...>{});
	}
};

template <typename Class, typename Return, typename... Args>
class CMethod<Class, Return(Class::*)(Args...) noexcept> : public CMethodBase<Class, Return, Args...> {
private:
	Return(Class::* m_method)(Args...) noexcept;

public:
	CMethod(Return(Class::* method)(Args...) noexcept) : m_method(method) {
	}

	IAdaptor& Invoke(IAdaptor& adaptor, std::vector<IAdaptor*> args) {
		return CMethodBase<Class, Return, Args...>::Invoke(m_method, adaptor, args, std::index_sequence_for<Args...>{});
	}
};

template <typename Class, typename Return, typename... Args>
class CMethod<Class, Return(Class::*)(Args...) const noexcept> : public CMethodBase<Class, Return, Args...> {
private:
	Return(Class::* m_method)(Args...) const noexcept;

public:
	CMethod(Return(Class::* method)(Args...) const noexcept) : m_method(method) {
	}

	IAdaptor& Invoke(IAdaptor& adaptor, std::vector<IAdaptor*> args) {
		return CMethodBase<Class, Return, Args...>::Invoke(m_method, adaptor, args, std::index_sequence_for<Args...>{});
	}
};

template <typename Class, typename Return, typename... Args>
class CMethod<Class, Return(Class::*)(Args...) volatile> : public CMethodBase<Class, Return, Args...> {
private:
	Return(Class::* m_method)(Args...) volatile;

public:
	CMethod(Return(Class::* method)(Args...) volatile) : m_method(method) {
	}

	IAdaptor& Invoke(IAdaptor& adaptor, std::vector<IAdaptor*> args) {
		return CMethodBase<Class, Return, Args...>::Invoke(m_method, adaptor, args, std::index_sequence_for<Args...>{});
	}
};

template <typename Class, typename Return, typename... Args>
class CMethod<Class, Return(Class::*)(Args...) volatile const> : public CMethodBase<Class, Return, Args...> {
private:
	Return(Class::* m_method)(Args...) volatile const;

public:
	CMethod(Return(Class::* method)(Args...) volatile const) : m_method(method) {
	}

	IAdaptor& Invoke(IAdaptor& adaptor, std::vector<IAdaptor*> args) {
		return CMethodBase<Class, Return, Args...>::Invoke(m_method, adaptor, args, std::index_sequence_for<Args...>{});
	}
};

template <typename Class, typename Return, typename... Args>
class CMethod<Class, Return(Class::*)(Args...) volatile noexcept> : public CMethodBase<Class, Return, Args...> {
private:
	Return(Class::* m_method)(Args...) volatile noexcept;

public:
	CMethod(Return(Class::* method)(Args...) volatile noexcept) : m_method(method) {
	}

	IAdaptor& Invoke(IAdaptor& adaptor, std::vector<IAdaptor*> args) {
		return CMethodBase<Class, Return, Args...>::Invoke(m_method, adaptor, args, std::index_sequence_for<Args...>{});
	}
};

template <typename Class, typename Return, typename... Args>
class CMethod<Class, Return(Class::*)(Args...) volatile const noexcept> : public CMethodBase<Class, Return, Args...> {
private:
	Return(Class::* m_method)(Args...) volatile const noexcept;

public:
	CMethod(Return(Class::* method)(Args...) volatile const noexcept) : m_method(method) {
	}

	IAdaptor& Invoke(IAdaptor& adaptor, std::vector<IAdaptor*> args) {
		return CMethodBase<Class, Return, Args...>::Invoke(m_method, adaptor, args, std::index_sequence_for<Args...>{});
	}
};

template <typename Class, typename Return, typename... Args>
class CMethod<Class, Return(Class::*)(Args...) &> : public CMethodBase<Class, Return, Args...> {
private:
	Return(Class::* m_method)(Args...) &;

public:
	CMethod(Return(Class::* method)(Args...) &) : m_method(method) {
	}

	IAdaptor& Invoke(IAdaptor& adaptor, std::vector<IAdaptor*> args) {
		return CMethodBase<Class, Return, Args...>::Invoke(m_method, adaptor, args, std::index_sequence_for<Args...>{});
	}
};

template <typename Class, typename Return, typename... Args>
class CMethod<Class, Return(Class::*)(Args...) const &> : public CMethodBase<Class, Return, Args...> {
private:
	Return(Class::* m_method)(Args...) const &;

public:
	CMethod(Return(Class::* method)(Args...) const &) : m_method(method) {
	}

	IAdaptor& Invoke(IAdaptor& adaptor, std::vector<IAdaptor*> args) {
		return CMethodBase<Class, Return, Args...>::Invoke(m_method, adaptor, args, std::index_sequence_for<Args...>{});
	}
};

template <typename Class, typename Return, typename... Args>
class CMethod<Class, Return(Class::*)(Args...) & noexcept> : public CMethodBase<Class, Return, Args...> {
private:
	Return(Class::* m_method)(Args...) & noexcept;

public:
	CMethod(Return(Class::* method)(Args...) & noexcept) : m_method(method) {
	}

	IAdaptor& Invoke(IAdaptor& adaptor, std::vector<IAdaptor*> args) {
		return CMethodBase<Class, Return, Args...>::Invoke(m_method, adaptor, args, std::index_sequence_for<Args...>{});
	}
};

template <typename Class, typename Return, typename... Args>
class CMethod<Class, Return(Class::*)(Args...) const & noexcept> : public CMethodBase<Class, Return, Args...> {
private:
	Return(Class::* m_method)(Args...) const & noexcept;

public:
	CMethod(Return(Class::* method)(Args...) const & noexcept) : m_method(method) {
	}

	IAdaptor& Invoke(IAdaptor& adaptor, std::vector<IAdaptor*> args) {
		return CMethodBase<Class, Return, Args...>::Invoke(m_method, adaptor, args, std::index_sequence_for<Args...>{});
	}
};

template <typename Class, typename Return, typename... Args>
class CMethod<Class, Return(Class::*)(Args...) volatile &> : public CMethodBase<Class, Return, Args...> {
private:
	Return(Class::* m_method)(Args...) volatile &;

public:
	CMethod(Return(Class::* method)(Args...) volatile &) : m_method(method) {
	}

	IAdaptor& Invoke(IAdaptor& adaptor, std::vector<IAdaptor*> args) {
		return CMethodBase<Class, Return, Args...>::Invoke(m_method, adaptor, args, std::index_sequence_for<Args...>{});
	}
};

template <typename Class, typename Return, typename... Args>
class CMethod<Class, Return(Class::*)(Args...) volatile const &> : public CMethodBase<Class, Return, Args...> {
private:
	Return(Class::* m_method)(Args...) volatile const &;

public:
	CMethod(Return(Class::* method)(Args...) volatile const &) : m_method(method) {
	}

	IAdaptor& Invoke(IAdaptor& adaptor, std::vector<IAdaptor*> args) {
		return CMethodBase<Class, Return, Args...>::Invoke(m_method, adaptor, args, std::index_sequence_for<Args...>{});
	}
};

template <typename Class, typename Return, typename... Args>
class CMethod<Class, Return(Class::*)(Args...) volatile & noexcept> : public CMethodBase<Class, Return, Args...> {
private:
	Return(Class::* m_method)(Args...) volatile & noexcept;

public:
	CMethod(Return(Class::* method)(Args...) volatile & noexcept) : m_method(method) {
	}

	IAdaptor& Invoke(IAdaptor& adaptor, std::vector<IAdaptor*> args) {
		return CMethodBase<Class, Return, Args...>::Invoke(m_method, adaptor, args, std::index_sequence_for<Args...>{});
	}
};

template <typename Class, typename Return, typename... Args>
class CMethod<Class, Return(Class::*)(Args...) volatile const & noexcept> : public CMethodBase<Class, Return, Args...> {
private:
	Return(Class::* m_method)(Args...) volatile const & noexcept;

public:
	CMethod(Return(Class::* method)(Args...) volatile const & noexcept) : m_method(method) {
	}

	IAdaptor& Invoke(IAdaptor& adaptor, std::vector<IAdaptor*> args) {
		return CMethodBase<Class, Return, Args...>::Invoke(m_method, adaptor, args, std::index_sequence_for<Args...>{});
	}
};

template <typename Class, typename Return, typename... Args>
class CMethod<Class, Return(Class::*)(Args...) &&> : public CMethodBase<Class, Return, Args...> {
private:
	Return(Class::* m_method)(Args...) &&;

public:
	CMethod(Return(Class::* method)(Args...) &&) : m_method(method) {
	}

	IAdaptor& Invoke(IAdaptor& adaptor, std::vector<IAdaptor*> args) {
		return CMethodBase<Class, Return, Args...>::InvokeRValue(m_method, adaptor, args, std::index_sequence_for<Args...>{});
	}
};

template <typename Class, typename Return, typename... Args>
class CMethod<Class, Return(Class::*)(Args...) const &&> : public CMethodBase<Class, Return, Args...> {
private:
	Return(Class::* m_method)(Args...) const &&;

public:
	CMethod(Return(Class::* method)(Args...) const &&) : m_method(method) {
	}

	IAdaptor& Invoke(IAdaptor& adaptor, std::vector<IAdaptor*> args) {
		return CMethodBase<Class, Return, Args...>::InvokeRValue(m_method, adaptor, args, std::index_sequence_for<Args...>{});
	}
};

template <typename Class, typename Return, typename... Args>
class CMethod<Class, Return(Class::*)(Args...) && noexcept> : public CMethodBase<Class, Return, Args...> {
private:
	Return(Class::* m_method)(Args...) && noexcept;

public:
	CMethod(Return(Class::* method)(Args...) && noexcept) : m_method(method) {
	}

	IAdaptor& Invoke(IAdaptor& adaptor, std::vector<IAdaptor*> args) {
		return CMethodBase<Class, Return, Args...>::InvokeRValue(m_method, adaptor, args, std::index_sequence_for<Args...>{});
	}
};

template <typename Class, typename Return, typename... Args>
class CMethod<Class, Return(Class::*)(Args...) const && noexcept> : public CMethodBase<Class, Return, Args...> {
private:
	Return(Class::* m_method)(Args...) const && noexcept;

public:
	CMethod(Return(Class::* method)(Args...) const && noexcept) : m_method(method) {
	}

	IAdaptor& Invoke(IAdaptor& adaptor, std::vector<IAdaptor*> args) {
		return CMethodBase<Class, Return, Args...>::InvokeRValue(m_method, adaptor, args, std::index_sequence_for<Args...>{});
	}
};

template <typename Class, typename Return, typename... Args>
class CMethod<Class, Return(Class::*)(Args...) volatile &&> : public CMethodBase<Class, Return, Args...> {
private:
	Return(Class::* m_method)(Args...) volatile &&;

public:
	CMethod(Return(Class::* method)(Args...) volatile &&) : m_method(method) {
	}

	IAdaptor& Invoke(IAdaptor& adaptor, std::vector<IAdaptor*> args) {
		return CMethodBase<Class, Return, Args...>::InvokeRValue(m_method, adaptor, args, std::index_sequence_for<Args...>{});
	}
};

template <typename Class, typename Return, typename... Args>
class CMethod<Class, Return(Class::*)(Args...) volatile const &&> : public CMethodBase<Class, Return, Args...> {
private:
	Return(Class::* m_method)(Args...) volatile const &&;

public:
	CMethod(Return(Class::* method)(Args...) volatile const &&) : m_method(method) {
	}

	IAdaptor& Invoke(IAdaptor& adaptor, std::vector<IAdaptor*> args) {
		return CMethodBase<Class, Return, Args...>::InvokeRValue(m_method, adaptor, args, std::index_sequence_for<Args...>{});
	}
};

template <typename Class, typename Return, typename... Args>
class CMethod<Class, Return(Class::*)(Args...) volatile && noexcept> : public CMethodBase<Class, Return, Args...> {
private:
	Return(Class::* m_method)(Args...) volatile && noexcept;

public:
	CMethod(Return(Class::* method)(Args...) volatile && noexcept) : m_method(method) {
	}

	IAdaptor& Invoke(IAdaptor& adaptor, std::vector<IAdaptor*> args) {
		return CMethodBase<Class, Return, Args...>::InvokeRValue(m_method, adaptor, args, std::index_sequence_for<Args...>{});
	}
};

template <typename Class, typename Return, typename... Args>
class CMethod<Class, Return(Class::*)(Args...) volatile const && noexcept> : public CMethodBase<Class, Return, Args...> {
private:
	Return(Class::* m_method)(Args...) volatile const && noexcept;

public:
	CMethod(Return(Class::* method)(Args...) volatile const && noexcept) : m_method(method) {
	}

	IAdaptor& Invoke(IAdaptor& adaptor, std::vector<IAdaptor*> args) {
		return CMethodBase<Class, Return, Args...>::InvokeRValue(m_method, adaptor, args, std::index_sequence_for<Args...>{});
	}
};

