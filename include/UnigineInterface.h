/* Copyright (C) 2005-2022, UNIGINE. All rights reserved.
 *
 * This file is a part of the UNIGINE 2 SDK.
 *
 * Your use and / or redistribution of this software in source and / or
 * binary form, with or without modification, is subject to: (i) your
 * ongoing acceptance of and compliance with the terms and conditions of
 * the UNIGINE License Agreement; and (ii) your inclusion of this notice
 * in any version of this software that you use or redistribute.
 * A copy of the UNIGINE License Agreement is available by contacting
 * UNIGINE. at http://unigine.com/
 */


#pragma once

#include "UnigineInterpreter.h"

/**
 * Unigine namespace.
 */
namespace Unigine
{

/**************************************************************************\
*
* Extern variables
*
\**************************************************************************/

/// @cond

/*
 */
class UNIGINE_API ExternVariableBase
{
public:
	virtual ~ExternVariableBase() {}

private:

	friend class Interpreter;
	friend class ExternVariableExport;

	void *get_export();

	static void release_ptr(ExternVariableBase *ptr) { delete ptr; }

	virtual int is_constant() const = 0;

	virtual String get_info(const char *name) const = 0;

	virtual Variable get(void *i) const = 0;
};

/*
 */
template <class Type> class ExternVariable: public ExternVariableBase
{
public:
	ExternVariable(Type *v): variable(v) {}

private:
	virtual int is_constant() const { return 0; }

	virtual String get_info(const char *name) const
	{
	#ifdef UNIGINE_EXTERN_INFO
		return String::format("%s %s;\n", TypeToName<Type>::name(), name);
	#else
		return String();
	#endif
	}

	virtual Variable get(void *i) const
	{
		TypeToVariable<Type> ret(i, *variable);
		return ret.value;
	}

	Type *variable;
};

/*
 */
template <class Type> class ExternConstant: public ExternVariableBase
{
public:
	ExternConstant(Type v): variable(v) {}

private:
	virtual int is_constant() const { return 1; }

	virtual String get_info(const char *name) const
	{
	#ifdef UNIGINE_EXTERN_INFO
		return String::format("%s %s;\n", TypeToName<Type>::name(), name);
	#else
		return String();
	#endif
	}

	virtual Variable get(void *i) const
	{
		TypeToVariable<Type> ret(i, variable);
		return ret.value;
	}

	Type variable;
};

/// @endcond

/**
 * Make a pointer to an external variable.
 *
 * @param variable Pointer to the external variable. The variable can be of the following types: int, float, vec3, vec4, dvec3, dvec4, ivec3, ivec4, mat4, dmat4, quat, string, external class or Variable.
 * @return Pointer to the variable.
 */
template <class Type>
ExternVariableBase *MakeExternVariable(Type *variable)
{
	return new ExternVariable<Type>(variable);
}

/**
 * Make a pointer to an external constant.
 *
 * @param variable The value of the external variable.
 * @tparam Type Type of the variable, the supported types are: int, float, vec3, vec4, dvec3, dvec4, ivec3, ivec4, mat4, dmat4, quat, string, external class or Variable.
 * @return Pointer to the constant.
 */
template <class Type>
ExternVariableBase *MakeExternConstant(Type variable)
{
	return new ExternConstant<Type>(variable);
}

/**************************************************************************\
*
* Extern functions
*
\**************************************************************************/

/// @cond

/*
 */
class UNIGINE_API ExternFunctionBase
{
public:
	virtual ~ExternFunctionBase() {}

private:
	friend class Interpreter;
	friend class ExternFunctionExport;

	void *get_export();

	static void release_ptr(ExternFunctionBase *ptr) { delete ptr; }

	virtual TypeInfo get_type() const = 0;

	virtual int get_num_args() const = 0;
	virtual const char *get_args() const = 0;

	virtual String get_info(const char *name) const = 0;

	virtual Variable run(void *i, Variable *args) const = 0;
};

/*
 */
template <class TList, class Func> class ExternFunction: public ExternFunctionBase
{
public:
	ExternFunction(Func func, const char *args): func(func), args(args) {}

private:
	enum { Length = Length<TList>::Value - 1 };

	typedef typename TypeAt<TList, 0>::Type Ret;
	typedef typename TypeAt<TList, 1>::Type A0;
	typedef typename TypeAt<TList, 2>::Type A1;
	typedef typename TypeAt<TList, 3>::Type A2;
	typedef typename TypeAt<TList, 4>::Type A3;
	typedef typename TypeAt<TList, 5>::Type A4;
	typedef typename TypeAt<TList, 6>::Type A5;
	typedef typename TypeAt<TList, 7>::Type A6;
	typedef typename TypeAt<TList, 8>::Type A7;
	typedef typename TypeAt<TList, 9>::Type A8;

	virtual TypeInfo get_type() const
	{
		return TypeInfo(TypeID<Ret>());
	}

	virtual int get_num_args() const { return Length; }
	virtual const char *get_args() const { return args.get(); }

	virtual String get_info(const char *name) const
	{
		#ifdef UNIGINE_EXTERN_INFO
			return do_get_info(name, IntToType<Length>());
		#else
			return String();
		#endif
	}

	#ifdef UNIGINE_EXTERN_INFO
		String do_get_info(const char *name, IntToType<0>) const
		{
			return String::format("%s %s();\n", TypeToName<Ret>::name(), name);
		}
		String do_get_info(const char *name, IntToType<1>) const
		{
			return String::format("%s %s(%s);\n", TypeToName<Ret>::name(), name,
								  TypeToName<A0>::name());
		}
		String do_get_info(const char *name, IntToType<2>) const
		{
			return String::format("%s %s(%s,%s);\n", TypeToName<Ret>::name(), name,
								  TypeToName<A0>::name(), TypeToName<A1>::name());
		}
		String do_get_info(const char *name, IntToType<3>) const
		{
			return String::format("%s %s(%s,%s,%s);\n", TypeToName<Ret>::name(), name,
								  TypeToName<A0>::name(), TypeToName<A1>::name(), TypeToName<A2>::name());
		}
		String do_get_info(const char *name, IntToType<4>) const
		{
			return String::format("%s %s(%s,%s,%s,%s);\n", TypeToName<Ret>::name(), name,
								  TypeToName<A0>::name(), TypeToName<A1>::name(), TypeToName<A2>::name(), TypeToName<A3>::name());
		}
		String do_get_info(const char *name, IntToType<5>) const
		{
			return String::format("%s %s(%s,%s,%s,%s,%s);\n", TypeToName<Ret>::name(), name,
								  TypeToName<A0>::name(), TypeToName<A1>::name(), TypeToName<A2>::name(), TypeToName<A3>::name(),
								  TypeToName<A4>::name());
		}
		String do_get_info(const char *name, IntToType<6>) const
		{
			return String::format("%s %s(%s,%s,%s,%s,%s,%s);\n", TypeToName<Ret>::name(), name,
								  TypeToName<A0>::name(), TypeToName<A1>::name(), TypeToName<A2>::name(), TypeToName<A3>::name(),
								  TypeToName<A4>::name(), TypeToName<A5>::name());
		}
		String do_get_info(const char *name, IntToType<7>) const
		{
			return String::format("%s %s(%s,%s,%s,%s,%s,%s,%s);\n", TypeToName<Ret>::name(), name,
								  TypeToName<A0>::name(), TypeToName<A1>::name(), TypeToName<A2>::name(), TypeToName<A3>::name(),
								  TypeToName<A4>::name(), TypeToName<A5>::name(), TypeToName<A6>::name());
		}
		String do_get_info(const char *name, IntToType<8>) const
		{
			return String::format("%s %s(%s,%s,%s,%s,%s,%s,%s,%s);\n", TypeToName<Ret>::name(), name,
								  TypeToName<A0>::name(), TypeToName<A1>::name(), TypeToName<A2>::name(), TypeToName<A3>::name(),
								  TypeToName<A4>::name(), TypeToName<A5>::name(), TypeToName<A6>::name(), TypeToName<A7>::name());
		}
		String do_get_info(const char *name, IntToType<9>) const
		{
			return String::format("%s %s(%s,%s,%s,%s,%s,%s,%s,%s,%s);\n", TypeToName<Ret>::name(), name,
								  TypeToName<A0>::name(), TypeToName<A1>::name(), TypeToName<A2>::name(), TypeToName<A3>::name(),
								  TypeToName<A4>::name(), TypeToName<A5>::name(), TypeToName<A6>::name(), TypeToName<A7>::name(),
								  TypeToName<A8>::name());
		}
	#endif

	virtual Variable run(void *i, Variable *args_array) const
	{
		return do_run(i, args_array, TypeToType<Ret>(), IntToType<Length>());
	}
	template <class Type> UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, Type, IntToType<0>) const
	{
		UNIGINE_UNUSED(args_array);
		TypeToVariable<Ret> ret(i, func());
		return ret.value;
	}
	UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, TypeToType<void>, IntToType<0>) const
	{
		UNIGINE_UNUSED(args_array);
		func();
		return Variable();
	}
	template <class Type> UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, Type, IntToType<1>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		TypeToVariable<Ret> ret(i, func(a0.value));
		return ret.value;
	}
	UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, TypeToType<void>, IntToType<1>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		func(a0.value);
		return Variable();
	}
	template <class Type> UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, Type, IntToType<2>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		TypeToVariable<Ret> ret(i, func(a0.value, a1.value));
		return ret.value;
	}
	UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, TypeToType<void>, IntToType<2>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		func(a0.value, a1.value);
		return Variable();
	}
	template <class Type> UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, Type, IntToType<3>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		TypeToVariable<Ret> ret(i, func(a0.value, a1.value, a2.value));
		return ret.value;
	}
	UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, TypeToType<void>, IntToType<3>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		func(a0.value, a1.value, a2.value);
		return Variable();
	}
	template <class Type> UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, Type, IntToType<4>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		TypeToVariable<Ret> ret(i, func(a0.value, a1.value, a2.value, a3.value));
		return ret.value;
	}
	UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, TypeToType<void>, IntToType<4>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		func(a0.value, a1.value, a2.value, a3.value);
		return Variable();
	}
	template <class Type> UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, Type, IntToType<5>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		TypeToVariable<Ret> ret(i, func(a0.value, a1.value, a2.value, a3.value, a4.value));
		return ret.value;
	}
	UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, TypeToType<void>, IntToType<5>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		func(a0.value, a1.value, a2.value, a3.value, a4.value);
		return Variable();
	}
	template <class Type> UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, Type, IntToType<6>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		VariableToType<A5> a5(i, args_array[5]);
		TypeToVariable<Ret> ret(i, func(a0.value, a1.value, a2.value, a3.value, a4.value, a5.value));
		return ret.value;
	}
	UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, TypeToType<void>, IntToType<6>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		VariableToType<A5> a5(i, args_array[5]);
		func(a0.value, a1.value, a2.value, a3.value, a4.value, a5.value);
		return Variable();
	}
	template <class Type> UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, Type, IntToType<7>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		VariableToType<A5> a5(i, args_array[5]);
		VariableToType<A6> a6(i, args_array[6]);
		TypeToVariable<Ret> ret(i, func(a0.value, a1.value, a2.value, a3.value, a4.value, a5.value, a6.value));
		return ret.value;
	}
	UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, TypeToType<void>, IntToType<7>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		VariableToType<A5> a5(i, args_array[5]);
		VariableToType<A6> a6(i, args_array[6]);
		func(a0.value, a1.value, a2.value, a3.value, a4.value, a5.value, a6.value);
		return Variable();
	}
	template <class Type> UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, Type, IntToType<8>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		VariableToType<A5> a5(i, args_array[5]);
		VariableToType<A6> a6(i, args_array[6]);
		VariableToType<A7> a7(i, args_array[7]);
		TypeToVariable<Ret> ret(i, func(a0.value, a1.value, a2.value, a3.value, a4.value, a5.value, a6.value, a7.value));
		return ret.value;
	}
	UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, TypeToType<void>, IntToType<8>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		VariableToType<A5> a5(i, args_array[5]);
		VariableToType<A6> a6(i, args_array[6]);
		VariableToType<A7> a7(i, args_array[7]);
		func(a0.value, a1.value, a2.value, a3.value, a4.value, a5.value, a6.value, a7.value);
		return Variable();
	}
	template <class Type> UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, Type, IntToType<9>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		VariableToType<A5> a5(i, args_array[5]);
		VariableToType<A6> a6(i, args_array[6]);
		VariableToType<A7> a7(i, args_array[7]);
		VariableToType<A8> a8(i, args_array[8]);
		TypeToVariable<Ret> ret(i, func(a0.value, a1.value, a2.value, a3.value, a4.value, a5.value, a6.value, a7.value, a8.value));
		return ret.value;
	}
	UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, TypeToType<void>, IntToType<9>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		VariableToType<A5> a5(i, args_array[5]);
		VariableToType<A6> a6(i, args_array[6]);
		VariableToType<A7> a7(i, args_array[7]);
		VariableToType<A8> a8(i, args_array[8]);
		func(a0.value, a1.value, a2.value, a3.value, a4.value, a5.value, a6.value, a7.value, a8.value);
		return Variable();
	}

	Func func;
	String args;
};

/// @endcond

/**
 * Make a pointer to an external function. The function can receive up to 9 arguments and return void or any value of supported type.
 *
 * @param func Pointer to the external function.
 * @param args Default arguments.
 * @return Pointer to the function.
 */
template <class Ret>
ExternFunctionBase *MakeExternFunction(Ret(*func)(), const char *args = 0)
{
	return new ExternFunction<typename MakeTypeList<Ret>::Type, Ret(*)()>(func, args);
}

/// @cond

template <class Ret, class A0>
ExternFunctionBase *MakeExternFunction(Ret(*func)(A0), const char *args = 0)
{
	return new ExternFunction<typename MakeTypeList<Ret, A0>::Type, Ret(*)(A0)>(func, args);
}

template <class Ret, class A0, class A1>
ExternFunctionBase *MakeExternFunction(Ret(*func)(A0, A1), const char *args = 0)
{
	return new ExternFunction<typename MakeTypeList<Ret, A0, A1>::Type, Ret(*)(A0, A1)>(func, args);
}

template <class Ret, class A0, class A1, class A2>
ExternFunctionBase *MakeExternFunction(Ret(*func)(A0, A1, A2), const char *args = 0)
{
	return new ExternFunction<typename MakeTypeList<Ret, A0, A1, A2>::Type, Ret(*)(A0, A1, A2)>(func, args);
}

template <class Ret, class A0, class A1, class A2, class A3>
ExternFunctionBase *MakeExternFunction(Ret(*func)(A0, A1, A2, A3), const char *args = 0)
{
	return new ExternFunction<typename MakeTypeList<Ret, A0, A1, A2, A3>::Type, Ret(*)(A0, A1, A2, A3)>(func, args);
}

template <class Ret, class A0, class A1, class A2, class A3, class A4>
ExternFunctionBase *MakeExternFunction(Ret(*func)(A0, A1, A2, A3, A4), const char *args = 0)
{
	return new ExternFunction<typename MakeTypeList<Ret, A0, A1, A2, A3, A4>::Type, Ret(*)(A0, A1, A2, A3, A4)>(func, args);
}

template <class Ret, class A0, class A1, class A2, class A3, class A4, class A5>
ExternFunctionBase *MakeExternFunction(Ret(*func)(A0, A1, A2, A3, A4, A5), const char *args = 0)
{
	return new ExternFunction<typename MakeTypeList<Ret, A0, A1, A2, A3, A4, A5>::Type, Ret(*)(A0, A1, A2, A3, A4, A5)>(func, args);
}

template <class Ret, class A0, class A1, class A2, class A3, class A4, class A5, class A6>
ExternFunctionBase *MakeExternFunction(Ret(*func)(A0, A1, A2, A3, A4, A5, A6), const char *args = 0)
{
	return new ExternFunction<typename MakeTypeList<Ret, A0, A1, A2, A3, A4, A5, A6>::Type, Ret(*)(A0, A1, A2, A3, A4, A5, A6)>(func, args);
}

template <class Ret, class A0, class A1, class A2, class A3, class A4, class A5, class A6, class A7>
ExternFunctionBase *MakeExternFunction(Ret(*func)(A0, A1, A2, A3, A4, A5, A6, A7), const char *args = 0)
{
	return new ExternFunction<typename MakeTypeList<Ret, A0, A1, A2, A3, A4, A5, A6, A7>::Type, Ret(*)(A0, A1, A2, A3, A4, A5, A6, A7)>(func, args);
}

template <class Ret, class A0, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
ExternFunctionBase *MakeExternFunction(Ret(*func)(A0, A1, A2, A3, A4, A5, A6, A7, A8), const char *args = 0)
{
	return new ExternFunction<typename MakeTypeList<Ret, A0, A1, A2, A3, A4, A5, A6, A7, A8>::Type, Ret(*)(A0, A1, A2, A3, A4, A5, A6, A7, A8)>(func, args);
}

template <class Ret, class A0, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9>
ExternFunctionBase *MakeExternFunction(Ret(*func)(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9), const char *args = 0)
{
	return new ExternFunction<typename MakeTypeList<Ret, A0, A1, A2, A3, A4, A5, A6, A7, A8, A9>::Type, Ret(*)(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9)>(func, args);
}

/// @endcond

/**************************************************************************\
*
* Extern object functions
*
\**************************************************************************/

/// @cond

/*
 */
template <class Class, class TList, class Func> class ExternObjectFunction: public ExternFunctionBase
{
public:
	ExternObjectFunction(Class *object, Func func, const char *args): object(object), func(func), args(args) {}

private:
	enum { Length = Length<TList>::Value - 1 };

	typedef typename TypeAt<TList,  0>::Type Ret;
	typedef typename TypeAt<TList,  1>::Type A0;
	typedef typename TypeAt<TList,  2>::Type A1;
	typedef typename TypeAt<TList,  3>::Type A2;
	typedef typename TypeAt<TList,  4>::Type A3;
	typedef typename TypeAt<TList,  5>::Type A4;
	typedef typename TypeAt<TList,  6>::Type A5;
	typedef typename TypeAt<TList,  7>::Type A6;
	typedef typename TypeAt<TList,  8>::Type A7;
	typedef typename TypeAt<TList,  9>::Type A8;
	typedef typename TypeAt<TList, 10>::Type A9;

	virtual TypeInfo get_type() const
	{
		return TypeInfo(TypeID<Ret>());
	}

	virtual int get_num_args() const
	{
		return Length;
	}
	virtual const char *get_args() const
	{
		return args.get();
	}

	virtual String get_info(const char *name) const
	{
		#ifdef UNIGINE_EXTERN_INFO
			return do_get_info(name, IntToType<Length>());
		#else
			return String();
		#endif
	}

	#ifdef UNIGINE_EXTERN_INFO
		String do_get_info(const char *name, IntToType<0>) const
		{
			return String::format("%s %s();\n", TypeToName<Ret>::name(), name);
		}
		String do_get_info(const char *name, IntToType<1>) const
		{
			return String::format("%s %s(%s);\n", TypeToName<Ret>::name(), name,
								  TypeToName<A0>::name());
		}
		String do_get_info(const char *name, IntToType<2>) const
		{
			return String::format("%s %s(%s,%s);\n", TypeToName<Ret>::name(), name,
								  TypeToName<A0>::name(), TypeToName<A1>::name());
		}
		String do_get_info(const char *name, IntToType<3>) const
		{
			return String::format("%s %s(%s,%s,%s);\n", TypeToName<Ret>::name(), name,
								  TypeToName<A0>::name(), TypeToName<A1>::name(), TypeToName<A2>::name());
		}
		String do_get_info(const char *name, IntToType<4>) const
		{
			return String::format("%s %s(%s,%s,%s,%s);\n", TypeToName<Ret>::name(), name,
								  TypeToName<A0>::name(), TypeToName<A1>::name(), TypeToName<A2>::name(), TypeToName<A3>::name());
		}
		String do_get_info(const char *name, IntToType<5>) const
		{
			return String::format("%s %s(%s,%s,%s,%s,%s);\n", TypeToName<Ret>::name(), name,
								  TypeToName<A0>::name(), TypeToName<A1>::name(), TypeToName<A2>::name(), TypeToName<A3>::name(),
								  TypeToName<A4>::name());
		}
		String do_get_info(const char *name, IntToType<6>) const
		{
			return String::format("%s %s(%s,%s,%s,%s,%s,%s);\n", TypeToName<Ret>::name(), name,
								  TypeToName<A0>::name(), TypeToName<A1>::name(), TypeToName<A2>::name(), TypeToName<A3>::name(),
								  TypeToName<A4>::name(), TypeToName<A5>::name());
		}
		String do_get_info(const char *name, IntToType<7>) const
		{
			return String::format("%s %s(%s,%s,%s,%s,%s,%s,%s);\n", TypeToName<Ret>::name(), name,
								  TypeToName<A0>::name(), TypeToName<A1>::name(), TypeToName<A2>::name(), TypeToName<A3>::name(),
								  TypeToName<A4>::name(), TypeToName<A5>::name(), TypeToName<A6>::name());
		}
		String do_get_info(const char *name, IntToType<8>) const
		{
			return String::format("%s %s(%s,%s,%s,%s,%s,%s,%s,%s);\n", TypeToName<Ret>::name(), name,
								  TypeToName<A0>::name(), TypeToName<A1>::name(), TypeToName<A2>::name(), TypeToName<A3>::name(),
								  TypeToName<A4>::name(), TypeToName<A5>::name(), TypeToName<A6>::name(), TypeToName<A7>::name());
		}
		String do_get_info(const char *name, IntToType<9>) const
		{
			return String::format("%s %s(%s,%s,%s,%s,%s,%s,%s,%s,%s);\n", TypeToName<Ret>::name(), name,
								  TypeToName<A0>::name(), TypeToName<A1>::name(), TypeToName<A2>::name(), TypeToName<A3>::name(),
								  TypeToName<A4>::name(), TypeToName<A5>::name(), TypeToName<A6>::name(), TypeToName<A7>::name(),
								  TypeToName<A8>::name());
		}
		String do_get_info(const char *name, IntToType<10>) const
		{
			return String::format("%s %s(%s,%s,%s,%s,%s,%s,%s,%s,%s,%s);\n", TypeToName<Ret>::name(), name,
				TypeToName<A0>::name(), TypeToName<A1>::name(), TypeToName<A2>::name(), TypeToName<A3>::name(),
				TypeToName<A4>::name(), TypeToName<A5>::name(), TypeToName<A6>::name(), TypeToName<A7>::name(),
				TypeToName<A8>::name(), TypeToName<A9>::name());
		}
	#endif

	virtual Variable run(void *i, Variable *args_array) const
	{
		return do_run(i, args_array, IntToType<IsClassFunction<Func>::Value>(), TypeToType<Ret>(), IntToType<Length>());
	}
	template <class Type> UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, IntToType<1>, Type, IntToType<0>) const
	{
		UNIGINE_UNUSED(args_array);
		TypeToVariable<Ret> ret(i, (object->*func)());
		return ret.value;
	}
	UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, IntToType<1>, TypeToType<void>, IntToType<0>) const
	{
		UNIGINE_UNUSED(args_array);
		UNIGINE_UNUSED(i);
		(object->*func)();
		return Variable();
	}
	template <class Type> UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, IntToType<0>, Type, IntToType<0>) const
	{
		UNIGINE_UNUSED(args_array);
		TypeToVariable<Ret> ret(i, func(object));
		return ret.value;
	}
	UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, IntToType<0>, TypeToType<void>, IntToType<0>) const
	{
		UNIGINE_UNUSED(args_array);
		func(object);
		return Variable();
	}
	template <class Type> UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, IntToType<1>, Type, IntToType<1>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		TypeToVariable<Ret> ret(i, (object->*func)(a0.value));
		return ret.value;
	}
	UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, IntToType<1>, TypeToType<void>, IntToType<1>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		(object->*func)(a0.value);
		return Variable();
	}
	template <class Type> UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, IntToType<0>, Type, IntToType<1>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		TypeToVariable<Ret> ret(i, func(object, a0.value));
		return ret.value;
	}
	UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, IntToType<0>, TypeToType<void>, IntToType<1>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		func(object, a0.value);
		return Variable();
	}
	template <class Type> UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, IntToType<1>, Type, IntToType<2>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		TypeToVariable<Ret> ret(i, (object->*func)(a0.value, a1.value));
		return ret.value;
	}
	UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, IntToType<1>, TypeToType<void>, IntToType<2>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		(object->*func)(a0.value, a1.value);
		return Variable();
	}
	template <class Type> UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, IntToType<0>, Type, IntToType<2>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		TypeToVariable<Ret> ret(i, func(object, a0.value, a1.value));
		return ret.value;
	}
	UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, IntToType<0>, TypeToType<void>, IntToType<2>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		func(object, a0.value, a1.value);
		return Variable();
	}
	template <class Type> UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, IntToType<1>, Type, IntToType<3>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		TypeToVariable<Ret> ret(i, (object->*func)(a0.value, a1.value, a2.value));
		return ret.value;
	}
	UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, IntToType<1>, TypeToType<void>, IntToType<3>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		(object->*func)(a0.value, a1.value, a2.value);
		return Variable();
	}
	template <class Type> UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, IntToType<0>, Type, IntToType<3>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		TypeToVariable<Ret> ret(i, func(object, a0.value, a1.value, a2.value));
		return ret.value;
	}
	UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, IntToType<0>, TypeToType<void>, IntToType<3>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		func(object, a0.value, a1.value, a2.value);
		return Variable();
	}
	template <class Type> UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, IntToType<1>, Type, IntToType<4>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		TypeToVariable<Ret> ret(i, (object->*func)(a0.value, a1.value, a2.value, a3.value));
		return ret.value;
	}
	UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, IntToType<1>, TypeToType<void>, IntToType<4>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		(object->*func)(a0.value, a1.value, a2.value, a3.value);
		return Variable();
	}
	template <class Type> UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, IntToType<0>, Type, IntToType<4>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		TypeToVariable<Ret> ret(i, func(object, a0.value, a1.value, a2.value, a3.value));
		return ret.value;
	}
	UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, IntToType<0>, TypeToType<void>, IntToType<4>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		func(object, a0.value, a1.value, a2.value, a3.value);
		return Variable();
	}
	template <class Type> UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, IntToType<1>, Type, IntToType<5>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		TypeToVariable<Ret> ret(i, (object->*func)(a0.value, a1.value, a2.value, a3.value, a4.value));
		return ret.value;
	}
	UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, IntToType<1>, TypeToType<void>, IntToType<5>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		(object->*func)(a0.value, a1.value, a2.value, a3.value, a4.value);
		return Variable();
	}
	template <class Type> UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, IntToType<0>, Type, IntToType<5>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		TypeToVariable<Ret> ret(i, func(object, a0.value, a1.value, a2.value, a3.value, a4.value));
		return ret.value;
	}
	UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, IntToType<0>, TypeToType<void>, IntToType<5>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		func(object, a0.value, a1.value, a2.value, a3.value, a4.value);
		return Variable();
	}
	template <class Type> UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, IntToType<1>, Type, IntToType<6>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		VariableToType<A5> a5(i, args_array[5]);
		TypeToVariable<Ret> ret(i, (object->*func)(a0.value, a1.value, a2.value, a3.value, a4.value, a5.value));
		return ret.value;
	}
	UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, IntToType<1>, TypeToType<void>, IntToType<6>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		VariableToType<A5> a5(i, args_array[5]);
		(object->*func)(a0.value, a1.value, a2.value, a3.value, a4.value, a5.value);
		return Variable();
	}
	template <class Type> UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, IntToType<0>, Type, IntToType<6>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		VariableToType<A5> a5(i, args_array[5]);
		TypeToVariable<Ret> ret(i, func(object, a0.value, a1.value, a2.value, a3.value, a4.value, a5.value));
		return ret.value;
	}
	UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, IntToType<0>, TypeToType<void>, IntToType<6>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		VariableToType<A5> a5(i, args_array[5]);
		func(object, a0.value, a1.value, a2.value, a3.value, a4.value, a5.value);
		return Variable();
	}
	template <class Type> UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, IntToType<1>, Type, IntToType<7>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		VariableToType<A5> a5(i, args_array[5]);
		VariableToType<A6> a6(i, args_array[6]);
		TypeToVariable<Ret> ret(i, (object->*func)(a0.value, a1.value, a2.value, a3.value, a4.value, a5.value, a6.value));
		return ret.value;
	}
	UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, IntToType<1>, TypeToType<void>, IntToType<7>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		VariableToType<A5> a5(i, args_array[5]);
		VariableToType<A6> a6(i, args_array[6]);
		(object->*func)(a0.value, a1.value, a2.value, a3.value, a4.value, a5.value, a6.value);
		return Variable();
	}
	template <class Type> UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, IntToType<0>, Type, IntToType<7>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		VariableToType<A5> a5(i, args_array[5]);
		VariableToType<A6> a6(i, args_array[6]);
		TypeToVariable<Ret> ret(i, func(object, a0.value, a1.value, a2.value, a3.value, a4.value, a5.value, a6.value));
		return ret.value;
	}
	UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, IntToType<0>, TypeToType<void>, IntToType<7>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		VariableToType<A5> a5(i, args_array[5]);
		VariableToType<A6> a6(i, args_array[6]);
		func(object, a0.value, a1.value, a2.value, a3.value, a4.value, a5.value, a6.value);
		return Variable();
	}
	template <class Type> UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, IntToType<1>, Type, IntToType<8>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		VariableToType<A5> a5(i, args_array[5]);
		VariableToType<A6> a6(i, args_array[6]);
		VariableToType<A7> a7(i, args_array[7]);
		TypeToVariable<Ret> ret(i, (object->*func)(a0.value, a1.value, a2.value, a3.value, a4.value, a5.value, a6.value, a7.value));
		return ret.value;
	}
	UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, IntToType<1>, TypeToType<void>, IntToType<8>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		VariableToType<A5> a5(i, args_array[5]);
		VariableToType<A6> a6(i, args_array[6]);
		VariableToType<A7> a7(i, args_array[7]);
		(object->*func)(a0.value, a1.value, a2.value, a3.value, a4.value, a5.value, a6.value, a7.value);
		return Variable();
	}
	template <class Type> UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, IntToType<0>, Type, IntToType<8>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		VariableToType<A5> a5(i, args_array[5]);
		VariableToType<A6> a6(i, args_array[6]);
		VariableToType<A7> a7(i, args_array[7]);
		TypeToVariable<Ret> ret(i, func(object, a0.value, a1.value, a2.value, a3.value, a4.value, a5.value, a6.value, a7.value));
		return ret.value;
	}
	UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, IntToType<0>, TypeToType<void>, IntToType<8>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		VariableToType<A5> a5(i, args_array[5]);
		VariableToType<A6> a6(i, args_array[6]);
		VariableToType<A7> a7(i, args_array[7]);
		func(object, a0.value, a1.value, a2.value, a3.value, a4.value, a5.value, a6.value, a7.value);
		return Variable();
	}
	template <class Type> UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, IntToType<1>, Type, IntToType<9>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		VariableToType<A5> a5(i, args_array[5]);
		VariableToType<A6> a6(i, args_array[6]);
		VariableToType<A7> a7(i, args_array[7]);
		VariableToType<A8> a8(i, args_array[8]);
		TypeToVariable<Ret> ret(i, (object->*func)(a0.value, a1.value, a2.value, a3.value, a4.value, a5.value, a6.value, a7.value, a8.value));
		return ret.value;
	}
	UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, IntToType<1>, TypeToType<void>, IntToType<9>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		VariableToType<A5> a5(i, args_array[5]);
		VariableToType<A6> a6(i, args_array[6]);
		VariableToType<A7> a7(i, args_array[7]);
		VariableToType<A8> a8(i, args_array[8]);
		(object->*func)(a0.value, a1.value, a2.value, a3.value, a4.value, a5.value, a6.value, a7.value, a8.value);
		return Variable();
	}
	template <class Type> UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, IntToType<0>, Type, IntToType<9>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		VariableToType<A5> a5(i, args_array[5]);
		VariableToType<A6> a6(i, args_array[6]);
		VariableToType<A7> a7(i, args_array[7]);
		VariableToType<A8> a8(i, args_array[8]);
		TypeToVariable<Ret> ret(i, func(object, a0.value, a1.value, a2.value, a3.value, a4.value, a5.value, a6.value, a7.value, a8.value));
		return ret.value;
	}
	UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, IntToType<0>, TypeToType<void>, IntToType<9>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		VariableToType<A5> a5(i, args_array[5]);
		VariableToType<A6> a6(i, args_array[6]);
		VariableToType<A7> a7(i, args_array[7]);
		VariableToType<A8> a8(i, args_array[8]);
		func(object, a0.value, a1.value, a2.value, a3.value, a4.value, a5.value, a6.value, a7.value, a8.value);
		return Variable();
	}
	template <class Type> UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, IntToType<1>, Type, IntToType<10>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		VariableToType<A5> a5(i, args_array[5]);
		VariableToType<A6> a6(i, args_array[6]);
		VariableToType<A7> a7(i, args_array[7]);
		VariableToType<A8> a8(i, args_array[8]);
		VariableToType<A9> a9(i, args_array[9]);
		TypeToVariable<Ret> ret(i, (object->*func)(a0.value, a1.value, a2.value, a3.value, a4.value, a5.value, a6.value, a7.value, a8.value, a9.value));
		return ret.value;
	}
	UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, IntToType<1>, TypeToType<void>, IntToType<10>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		VariableToType<A5> a5(i, args_array[5]);
		VariableToType<A6> a6(i, args_array[6]);
		VariableToType<A7> a7(i, args_array[7]);
		VariableToType<A8> a8(i, args_array[8]);
		VariableToType<A9> a9(i, args_array[9]);
		(object->*func)(a0.value, a1.value, a2.value, a3.value, a4.value, a5.value, a6.value, a7.value, a8.value, a9.value);
		return Variable();
	}
	template <class Type> UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, IntToType<0>, Type, IntToType<10>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		VariableToType<A5> a5(i, args_array[5]);
		VariableToType<A6> a6(i, args_array[6]);
		VariableToType<A7> a7(i, args_array[7]);
		VariableToType<A8> a8(i, args_array[8]);
		VariableToType<A9> a9(i, args_array[9]);
		TypeToVariable<Ret> ret(i, func(object, a0.value, a1.value, a2.value, a3.value, a4.value, a5.value, a6.value, a7.value, a8.value, a9.value));
		return ret.value;
	}
	UNIGINE_INLINE Variable do_run(void *i, Variable *args_array, IntToType<0>, TypeToType<void>, IntToType<10>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		VariableToType<A5> a5(i, args_array[5]);
		VariableToType<A6> a6(i, args_array[6]);
		VariableToType<A7> a7(i, args_array[7]);
		VariableToType<A8> a8(i, args_array[8]);
		VariableToType<A9> a9(i, args_array[9]);
		func(object, a0.value, a1.value, a2.value, a3.value, a4.value, a5.value, a6.value, a7.value, a8.value, a9.value);
		return Variable();
	}

	Class *object;
	Func func;
	String args;
};

/// @endcond

/**
 * Make a pointer to an external member function of the object. The function can receive up to 9 arguments and return void or any value of supported type.
 *
 * @param object Pointer to the class object.
 * @param func Pointer to the external class member function.
 * @param args Default arguments.
 * @tparam Class Type of the external class.
 * @tparam Ret Type of the return value.
 * @return Pointer to the function.
 */
template <class Class, class Type, class Ret>
ExternFunctionBase *MakeExternObjectFunction(Class *object, Ret(Type:: *func)(), const char *args = 0)
{
	return new ExternObjectFunction<Class, typename MakeTypeList<Ret>::Type, Ret(Type:: *)()>(object, func, args);
}

/// @cond

template <class Class, class Type, class Ret>
ExternFunctionBase *MakeExternObjectFunction(Class *object, Ret(Type:: *func)() const, const char *args = 0)
{
	return new ExternObjectFunction<Class, typename MakeTypeList<Ret>::Type, Ret(Type:: *)() const>(object, func, args);
}

template <class Class, class Type, class Ret>
ExternFunctionBase *MakeExternObjectFunction(Class *object, Ret(*func)(Type *), const char *args = 0)
{
	return new ExternObjectFunction<Class, typename MakeTypeList<Ret>::Type, Ret(*)(Type *)>(object, func, args);
}

template <class Class, class Type, class Ret, class A0>
ExternFunctionBase *MakeExternObjectFunction(Class *object, Ret(Type:: *func)(A0), const char *args = 0)
{
	return new ExternObjectFunction<Class, typename MakeTypeList<Ret, A0>::Type, Ret(Type:: *)(A0)>(object, func, args);
}

template <class Class, class Type, class Ret, class A0>
ExternFunctionBase *MakeExternObjectFunction(Class *object, Ret(Type:: *func)(A0) const, const char *args = 0)
{
	return new ExternObjectFunction<Class, typename MakeTypeList<Ret, A0>::Type, Ret(Type:: *)(A0) const>(object, func, args);
}

template <class Class, class Type, class Ret, class A0>
ExternFunctionBase *MakeExternObjectFunction(Class *object, Ret(*func)(Type *, A0), const char *args = 0)
{
	return new ExternObjectFunction<Class, typename MakeTypeList<Ret, A0>::Type, Ret(*)(Type *, A0)>(object, func, args);
}

template <class Class, class Type, class Ret, class A0, class A1>
ExternFunctionBase *MakeExternObjectFunction(Class *object, Ret(Type:: *func)(A0, A1), const char *args = 0)
{
	return new ExternObjectFunction<Class, typename MakeTypeList<Ret, A0, A1>::Type, Ret(Type:: *)(A0, A1)>(object, func, args);
}

template <class Class, class Type, class Ret, class A0, class A1>
ExternFunctionBase *MakeExternObjectFunction(Class *object, Ret(Type:: *func)(A0, A1) const, const char *args = 0)
{
	return new ExternObjectFunction<Class, typename MakeTypeList<Ret, A0, A1>::Type, Ret(Type:: *)(A0, A1) const>(object, func, args);
}

template <class Class, class Type, class Ret, class A0, class A1>
ExternFunctionBase *MakeExternObjectFunction(Class *object, Ret(*func)(Type *, A0, A1), const char *args = 0)
{
	return new ExternObjectFunction<Class, typename MakeTypeList<Ret, A0, A1>::Type, Ret(*)(Type *, A0, A1)>(object, func, args);
}

template <class Class, class Type, class Ret, class A0, class A1, class A2>
ExternFunctionBase *MakeExternObjectFunction(Class *object, Ret(Type:: *func)(A0, A1, A2), const char *args = 0)
{
	return new ExternObjectFunction<Class, typename MakeTypeList<Ret, A0, A1, A2>::Type, Ret(Type:: *)(A0, A1, A2)>(object, func, args);
}

template <class Class, class Type, class Ret, class A0, class A1, class A2>
ExternFunctionBase *MakeExternObjectFunction(Class *object, Ret(Type:: *func)(A0, A1, A2) const, const char *args = 0)
{
	return new ExternObjectFunction<Class, typename MakeTypeList<Ret, A0, A1, A2>::Type, Ret(Type:: *)(A0, A1, A2) const>(object, func, args);
}

template <class Class, class Type, class Ret, class A0, class A1, class A2>
ExternFunctionBase *MakeExternObjectFunction(Class *object, Ret(*func)(Type *, A0, A1, A2), const char *args = 0)
{
	return new ExternObjectFunction<Class, typename MakeTypeList<Ret, A0, A1, A2>::Type, Ret(*)(Type *, A0, A1, A2)>(object, func, args);
}

template <class Class, class Type, class Ret, class A0, class A1, class A2, class A3>
ExternFunctionBase *MakeExternObjectFunction(Class *object, Ret(Type:: *func)(A0, A1, A2, A3), const char *args = 0)
{
	return new ExternObjectFunction<Class, typename MakeTypeList<Ret, A0, A1, A2, A3>::Type, Ret(Type:: *)(A0, A1, A2, A3)>(object, func, args);
}

template <class Class, class Type, class Ret, class A0, class A1, class A2, class A3>
ExternFunctionBase *MakeExternObjectFunction(Class *object, Ret(Type:: *func)(A0, A1, A2, A3) const, const char *args = 0)
{
	return new ExternObjectFunction<Class, typename MakeTypeList<Ret, A0, A1, A2, A3>::Type, Ret(Type:: *)(A0, A1, A2, A3) const>(object, func, args);
}

template <class Class, class Type, class Ret, class A0, class A1, class A2, class A3>
ExternFunctionBase *MakeExternObjectFunction(Class *object, Ret(*func)(Type *, A0, A1, A2, A3), const char *args = 0)
{
	return new ExternObjectFunction<Class, typename MakeTypeList<Ret, A0, A1, A2, A3>::Type, Ret(*)(Type *, A0, A1, A2, A3)>(object, func, args);
}

template <class Class, class Type, class Ret, class A0, class A1, class A2, class A3, class A4>
ExternFunctionBase *MakeExternObjectFunction(Class *object, Ret(Type:: *func)(A0, A1, A2, A3, A4), const char *args = 0)
{
	return new ExternObjectFunction<Class, typename MakeTypeList<Ret, A0, A1, A2, A3, A4>::Type, Ret(Type:: *)(A0, A1, A2, A3, A4)>(object, func, args);
}

template <class Class, class Type, class Ret, class A0, class A1, class A2, class A3, class A4>
ExternFunctionBase *MakeExternObjectFunction(Class *object, Ret(Type:: *func)(A0, A1, A2, A3, A4) const, const char *args = 0)
{
	return new ExternObjectFunction<Class, typename MakeTypeList<Ret, A0, A1, A2, A3, A4>::Type, Ret(Type:: *)(A0, A1, A2, A3, A4) const>(object, func, args);
}

template <class Class, class Type, class Ret, class A0, class A1, class A2, class A3, class A4>
ExternFunctionBase *MakeExternObjectFunction(Class *object, Ret(*func)(Type *, A0, A1, A2, A3, A4), const char *args = 0)
{
	return new ExternObjectFunction<Class, typename MakeTypeList<Ret, A0, A1, A2, A3, A4>::Type, Ret(*)(Type *, A0, A1, A2, A3, A4)>(object, func, args);
}

template <class Class, class Type, class Ret, class A0, class A1, class A2, class A3, class A4, class A5>
ExternFunctionBase *MakeExternObjectFunction(Class *object, Ret(Type:: *func)(A0, A1, A2, A3, A4, A5), const char *args = 0)
{
	return new ExternObjectFunction<Class, typename MakeTypeList<Ret, A0, A1, A2, A3, A4, A5>::Type, Ret(Type:: *)(A0, A1, A2, A3, A4, A5)>(object, func, args);
}

template <class Class, class Type, class Ret, class A0, class A1, class A2, class A3, class A4, class A5>
ExternFunctionBase *MakeExternObjectFunction(Class *object, Ret(Type:: *func)(A0, A1, A2, A3, A4, A5) const, const char *args = 0)
{
	return new ExternObjectFunction<Class, typename MakeTypeList<Ret, A0, A1, A2, A3, A4, A5>::Type, Ret(Type:: *)(A0, A1, A2, A3, A4, A5) const>(object, func, args);
}

template <class Class, class Type, class Ret, class A0, class A1, class A2, class A3, class A4, class A5>
ExternFunctionBase *MakeExternObjectFunction(Class *object, Ret(*func)(Type *, A0, A1, A2, A3, A4, A5), const char *args = 0)
{
	return new ExternObjectFunction<Class, typename MakeTypeList<Ret, A0, A1, A2, A3, A4, A5>::Type, Ret(*)(Type *, A0, A1, A2, A3, A4, A5)>(object, func, args);
}

template <class Class, class Type, class Ret, class A0, class A1, class A2, class A3, class A4, class A5, class A6>
ExternFunctionBase *MakeExternObjectFunction(Class *object, Ret(Type:: *func)(A0, A1, A2, A3, A4, A5, A6), const char *args = 0)
{
	return new ExternObjectFunction<Class, typename MakeTypeList<Ret, A0, A1, A2, A3, A4, A5, A6>::Type, Ret(Type:: *)(A0, A1, A2, A3, A4, A5, A6)>(object, func, args);
}

template <class Class, class Type, class Ret, class A0, class A1, class A2, class A3, class A4, class A5, class A6>
ExternFunctionBase *MakeExternObjectFunction(Class *object, Ret(Type:: *func)(A0, A1, A2, A3, A4, A5, A6) const, const char *args = 0)
{
	return new ExternObjectFunction<Class, typename MakeTypeList<Ret, A0, A1, A2, A3, A4, A5, A6>::Type, Ret(Type:: *)(A0, A1, A2, A3, A4, A5, A6) const>(object, func, args);
}

template <class Class, class Type, class Ret, class A0, class A1, class A2, class A3, class A4, class A5, class A6>
ExternFunctionBase *MakeExternObjectFunction(Class *object, Ret(*func)(Type *, A0, A1, A2, A3, A4, A5, A6), const char *args = 0)
{
	return new ExternObjectFunction<Class, typename MakeTypeList<Ret, A0, A1, A2, A3, A4, A5, A6>::Type, Ret(*)(Type *, A0, A1, A2, A3, A4, A5, A6)>(object, func, args);
}

template <class Class, class Type, class Ret, class A0, class A1, class A2, class A3, class A4, class A5, class A6, class A7>
ExternFunctionBase *MakeExternObjectFunction(Class *object, Ret(Type:: *func)(A0, A1, A2, A3, A4, A5, A6, A7), const char *args = 0)
{
	return new ExternObjectFunction<Class, typename MakeTypeList<Ret, A0, A1, A2, A3, A4, A5, A6, A7>::Type, Ret(Type:: *)(A0, A1, A2, A3, A4, A5, A6, A7)>(object, func, args);
}

template <class Class, class Type, class Ret, class A0, class A1, class A2, class A3, class A4, class A5, class A6, class A7>
ExternFunctionBase *MakeExternObjectFunction(Class *object, Ret(Type:: *func)(A0, A1, A2, A3, A4, A5, A6, A7) const, const char *args = 0)
{
	return new ExternObjectFunction<Class, typename MakeTypeList<Ret, A0, A1, A2, A3, A4, A5, A6, A7>::Type, Ret(Type:: *)(A0, A1, A2, A3, A4, A5, A6, A7) const>(object, func, args);
}

template <class Class, class Type, class Ret, class A0, class A1, class A2, class A3, class A4, class A5, class A6, class A7>
ExternFunctionBase *MakeExternObjectFunction(Class *object, Ret(*func)(Type *, A0, A1, A2, A3, A4, A5, A6, A7), const char *args = 0)
{
	return new ExternObjectFunction<Class, typename MakeTypeList<Ret, A0, A1, A2, A3, A4, A5, A6, A7>::Type, Ret(*)(Type *, A0, A1, A2, A3, A4, A5, A6, A7)>(object, func, args);
}

template <class Class, class Type, class Ret, class A0, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
ExternFunctionBase *MakeExternObjectFunction(Class *object, Ret(Type:: *func)(A0, A1, A2, A3, A4, A5, A6, A7, A8), const char *args = 0)
{
	return new ExternObjectFunction<Class, typename MakeTypeList<Ret, A0, A1, A2, A3, A4, A5, A6, A7, A8>::Type, Ret(Type:: *)(A0, A1, A2, A3, A4, A5, A6, A7, A8)>(object, func, args);
}

template <class Class, class Type, class Ret, class A0, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
ExternFunctionBase *MakeExternObjectFunction(Class *object, Ret(Type:: *func)(A0, A1, A2, A3, A4, A5, A6, A7, A8) const, const char *args = 0)
{
	return new ExternObjectFunction<Class, typename MakeTypeList<Ret, A0, A1, A2, A3, A4, A5, A6, A7, A8>::Type, Ret(Type:: *)(A0, A1, A2, A3, A4, A5, A6, A7, A8) const>(object, func, args);
}

template <class Class, class Type, class Ret, class A0, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
ExternFunctionBase *MakeExternObjectFunction(Class *object, Ret(*func)(Type *, A0, A1, A2, A3, A4, A5, A6, A7, A8), const char *args = 0)
{
	return new ExternObjectFunction<Class, typename MakeTypeList<Ret, A0, A1, A2, A3, A4, A5, A6, A7, A8>::Type, Ret(*)(Type *, A0, A1, A2, A3, A4, A5, A6, A7, A8)>(object, func, args);
}

template <class Class, class Type, class Ret, class A0, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9>
ExternFunctionBase *MakeExternObjectFunction(Class *object, Ret(Type:: *func)(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9), const char *args = 0)
{
	return new ExternObjectFunction<Class, typename MakeTypeList<Ret, A0, A1, A2, A3, A4, A5, A6, A7, A8, A9>::Type, Ret(Type:: *)(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9)>(object, func, args);
}

template <class Class, class Type, class Ret, class A0, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9>
ExternFunctionBase *MakeExternObjectFunction(Class *object, Ret(Type:: *func)(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9) const, const char *args = 0)
{
	return new ExternObjectFunction<Class, typename MakeTypeList<Ret, A0, A1, A2, A3, A4, A5, A6, A7, A8, A9>::Type, Ret(Type:: *)(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9) const>(object, func, args);
}

template <class Class, class Type, class Ret, class A0, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9>
ExternFunctionBase *MakeExternObjectFunction(Class *object, Ret(*func)(Type *, A0, A1, A2, A3, A4, A5, A6, A7, A8, A9), const char *args = 0)
{
	return new ExternObjectFunction<Class, typename MakeTypeList<Ret, A0, A1, A2, A3, A4, A5, A6, A7, A8, A9>::Type, Ret(*)(Type *, A0, A1, A2, A3, A4, A5, A6, A7, A8, A9)>(object, func, args);
}

/// @endcond

/**************************************************************************\
*
* Extern class constructors
*
\**************************************************************************/

/// @cond

/*
 */
class UNIGINE_API ExternClassConstructorBase
{
public:
	virtual ~ExternClassConstructorBase() {}

private:
	template <class> friend class ExternClass;

	virtual int get_num_args() const = 0;
	virtual const char *get_args() const = 0;

	virtual String get_info(const char *name) const = 0;

	virtual void *run(void *i, Variable *args) const = 0;
};

/*
 */
template <class Class, class TList, class Func = int> class ExternClassConstructor: public ExternClassConstructorBase
{
public:
	ExternClassConstructor(const char *args): func(0), args(args) {}
	ExternClassConstructor(Func func, const char *args): func(func), args(args) {}

private:
	enum { Length = Length<TList>::Value };

	typedef typename TypeAt<TList, 0>::Type A0;
	typedef typename TypeAt<TList, 1>::Type A1;
	typedef typename TypeAt<TList, 2>::Type A2;
	typedef typename TypeAt<TList, 3>::Type A3;
	typedef typename TypeAt<TList, 4>::Type A4;
	typedef typename TypeAt<TList, 5>::Type A5;
	typedef typename TypeAt<TList, 6>::Type A6;
	typedef typename TypeAt<TList, 7>::Type A7;
	typedef typename TypeAt<TList, 8>::Type A8;
	typedef typename TypeAt<TList, 9>::Type A9;

	virtual int get_num_args() const
	{
		return Length;
	}
	virtual const char *get_args() const
	{
		return args.get();
	}

	virtual String get_info(const char *name) const
	{
	#ifdef UNIGINE_EXTERN_INFO
		return do_get_info(name, IntToType<Length>());
	#else
		return String();
	#endif
	}

	#ifdef UNIGINE_EXTERN_INFO
		String do_get_info(const char *name, IntToType<0>) const
		{
			return String::format("\t%s();\n", name);
		}
		String do_get_info(const char *name, IntToType<1>) const
		{
			return String::format("\t%s(%s);\n", name,
								  TypeToName<A0>::name());
		}
		String do_get_info(const char *name, IntToType<2>) const
		{
			return String::format("\t%s(%s,%s);\n", name,
								  TypeToName<A0>::name(), TypeToName<A1>::name());
		}
		String do_get_info(const char *name, IntToType<3>) const
		{
			return String::format("\t%s(%s,%s,%s);\n", name,
								  TypeToName<A0>::name(), TypeToName<A1>::name(), TypeToName<A2>::name());
		}
		String do_get_info(const char *name, IntToType<4>) const
		{
			return String::format("\t%s(%s,%s,%s,%s);\n", name,
								  TypeToName<A0>::name(), TypeToName<A1>::name(), TypeToName<A2>::name(), TypeToName<A3>::name());
		}
		String do_get_info(const char *name, IntToType<5>) const
		{
			return String::format("\t%s(%s,%s,%s,%s,%s);\n", name,
								  TypeToName<A0>::name(), TypeToName<A1>::name(), TypeToName<A2>::name(), TypeToName<A3>::name(),
								  TypeToName<A4>::name());
		}
		String do_get_info(const char *name, IntToType<6>) const
		{
			return String::format("\t%s(%s,%s,%s,%s,%s,%s);\n", name,
								  TypeToName<A0>::name(), TypeToName<A1>::name(), TypeToName<A2>::name(), TypeToName<A3>::name(),
								  TypeToName<A4>::name(), TypeToName<A5>::name());
		}
		String do_get_info(const char *name, IntToType<7>) const
		{
			return String::format("\t%s(%s,%s,%s,%s,%s,%s,%s);\n", name,
								  TypeToName<A0>::name(), TypeToName<A1>::name(), TypeToName<A2>::name(), TypeToName<A3>::name(),
								  TypeToName<A4>::name(), TypeToName<A5>::name(), TypeToName<A6>::name());
		}
		String do_get_info(const char *name, IntToType<8>) const
		{
			return String::format("\t%s(%s,%s,%s,%s,%s,%s,%s,%s);\n", name,
								  TypeToName<A0>::name(), TypeToName<A1>::name(), TypeToName<A2>::name(), TypeToName<A3>::name(),
								  TypeToName<A4>::name(), TypeToName<A5>::name(), TypeToName<A6>::name(), TypeToName<A7>::name());
		}
		String do_get_info(const char *name, IntToType<9>) const
		{
			return String::format("\t%s(%s,%s,%s,%s,%s,%s,%s,%s,%s);\n", name,
								  TypeToName<A0>::name(), TypeToName<A1>::name(), TypeToName<A2>::name(), TypeToName<A3>::name(),
								  TypeToName<A4>::name(), TypeToName<A5>::name(), TypeToName<A6>::name(), TypeToName<A7>::name(),
								  TypeToName<A8>::name());
		}
		String do_get_info(const char *name, IntToType<10>) const
		{
			return String::format("\t%s(%s,%s,%s,%s,%s,%s,%s,%s,%s,%s);\n", name,
				TypeToName<A0>::name(), TypeToName<A1>::name(), TypeToName<A2>::name(), TypeToName<A3>::name(),
				TypeToName<A4>::name(), TypeToName<A5>::name(), TypeToName<A6>::name(), TypeToName<A7>::name(),
				TypeToName<A8>::name(), TypeToName<A9>::name());
		}
	#endif

	virtual void *run(void *i, Variable *arguments) const
	{
		return do_run(i, arguments, TypeToType<Func>(), IntToType<Length>());
	}
	template <class Type> UNIGINE_INLINE void *do_run(void *i, Variable *args_array, Type, IntToType<0>) const
	{
		UNIGINE_UNUSED(args_array);
		return func();
	}
	UNIGINE_INLINE void *do_run(void *i, Variable *arguments, TypeToType<int>, IntToType<0>) const
	{
		UNIGINE_UNUSED(arguments);
		UNIGINE_UNUSED(i);
		return new Class();
	}
	template <class Type> UNIGINE_INLINE void *do_run(void *i, Variable *args_array, Type, IntToType<1>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		return func(a0.value);
	}
	UNIGINE_INLINE void *do_run(void *i, Variable *args_array, TypeToType<int>, IntToType<1>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		return new Class(a0.value);
	}
	template <class Type> UNIGINE_INLINE void *do_run(void *i, Variable *args_array, Type, IntToType<2>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		return func(a0.value, a1.value);
	}
	UNIGINE_INLINE void *do_run(void *i, Variable *args_array, TypeToType<int>, IntToType<2>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		return new Class(a0.value, a1.value);
	}
	template <class Type> UNIGINE_INLINE void *do_run(void *i, Variable *args_array, Type, IntToType<3>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		return func(a0.value, a1.value, a2.value);
	}
	UNIGINE_INLINE void *do_run(void *i, Variable *args_array, TypeToType<int>, IntToType<3>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		return new Class(a0.value, a1.value, a2.value);
	}
	template <class Type> UNIGINE_INLINE void *do_run(void *i, Variable *args_array, Type, IntToType<4>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		return func(a0.value, a1.value, a2.value, a3.value);
	}
	UNIGINE_INLINE void *do_run(void *i, Variable *args_array, TypeToType<int>, IntToType<4>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		return new Class(a0.value, a1.value, a2.value, a3.value);
	}
	template <class Type> UNIGINE_INLINE void *do_run(void *i, Variable *args_array, Type, IntToType<5>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		return func(a0.value, a1.value, a2.value, a3.value, a4.value);
	}
	UNIGINE_INLINE void *do_run(void *i, Variable *args_array, TypeToType<int>, IntToType<5>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		return new Class(a0.value, a1.value, a2.value, a3.value, a4.value);
	}
	template <class Type> UNIGINE_INLINE void *do_run(void *i, Variable *args_array, Type, IntToType<6>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		VariableToType<A5> a5(i, args_array[5]);
		return func(a0.value, a1.value, a2.value, a3.value, a4.value, a5.value);
	}
	UNIGINE_INLINE void *do_run(void *i, Variable *args_array, TypeToType<int>, IntToType<6>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		VariableToType<A5> a5(i, args_array[5]);
		return new Class(a0.value, a1.value, a2.value, a3.value, a4.value, a5.value);
	}
	template <class Type> UNIGINE_INLINE void *do_run(void *i, Variable *args_array, Type, IntToType<7>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		VariableToType<A5> a5(i, args_array[5]);
		VariableToType<A6> a6(i, args_array[6]);
		return func(a0.value, a1.value, a2.value, a3.value, a4.value, a5.value, a6.value);
	}
	UNIGINE_INLINE void *do_run(void *i, Variable *args_array, TypeToType<int>, IntToType<7>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		VariableToType<A5> a5(i, args_array[5]);
		VariableToType<A6> a6(i, args_array[6]);
		return new Class(a0.value, a1.value, a2.value, a3.value, a4.value, a5.value, a6.value);
	}
	template <class Type> UNIGINE_INLINE void *do_run(void *i, Variable *args_array, Type, IntToType<8>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		VariableToType<A5> a5(i, args_array[5]);
		VariableToType<A6> a6(i, args_array[6]);
		VariableToType<A7> a7(i, args_array[7]);
		return func(a0.value, a1.value, a2.value, a3.value, a4.value, a5.value, a6.value, a7.value);
	}
	UNIGINE_INLINE void *do_run(void *i, Variable *args_array, TypeToType<int>, IntToType<8>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		VariableToType<A5> a5(i, args_array[5]);
		VariableToType<A6> a6(i, args_array[6]);
		VariableToType<A7> a7(i, args_array[7]);
		return new Class(a0.value, a1.value, a2.value, a3.value, a4.value, a5.value, a6.value, a7.value);
	}
	template <class Type> UNIGINE_INLINE void *do_run(void *i, Variable *args_array, Type, IntToType<9>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		VariableToType<A5> a5(i, args_array[5]);
		VariableToType<A6> a6(i, args_array[6]);
		VariableToType<A7> a7(i, args_array[7]);
		VariableToType<A8> a8(i, args_array[8]);
		return func(a0.value, a1.value, a2.value, a3.value, a4.value, a5.value, a6.value, a7.value, a8.value);
	}
	UNIGINE_INLINE void *do_run(void *i, Variable *args_array, TypeToType<int>, IntToType<9>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		VariableToType<A5> a5(i, args_array[5]);
		VariableToType<A6> a6(i, args_array[6]);
		VariableToType<A7> a7(i, args_array[7]);
		VariableToType<A8> a8(i, args_array[8]);
		return new Class(a0.value, a1.value, a2.value, a3.value, a4.value, a5.value, a6.value, a7.value, a8.value);
	}
	template <class Type> UNIGINE_INLINE void *do_run(void *i, Variable *args_array, Type, IntToType<10>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		VariableToType<A5> a5(i, args_array[5]);
		VariableToType<A6> a6(i, args_array[6]);
		VariableToType<A7> a7(i, args_array[7]);
		VariableToType<A8> a8(i, args_array[8]);
		VariableToType<A9> a9(i, args_array[9]);
		return func(a0.value, a1.value, a2.value, a3.value, a4.value, a5.value, a6.value, a7.value, a8.value, a9.value);
	}
	UNIGINE_INLINE void *do_run(void *i, Variable *args_array, TypeToType<int>, IntToType<10>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		VariableToType<A5> a5(i, args_array[5]);
		VariableToType<A6> a6(i, args_array[6]);
		VariableToType<A7> a7(i, args_array[7]);
		VariableToType<A8> a8(i, args_array[8]);
		VariableToType<A9> a9(i, args_array[9]);
		return new Class(a0.value, a1.value, a2.value, a3.value, a4.value, a5.value, a6.value, a7.value, a8.value, a9.value);
	}

	Func func;
	String args;
};

/// @endcond

/**************************************************************************\
*
* Extern class functions
*
\**************************************************************************/

/// @cond

/*
 */
class UNIGINE_API ExternClassFunctionBase
{
public:
	virtual ~ExternClassFunctionBase() {}

private:
	template <class> friend class ExternClass;

	virtual const char *get_name() const = 0;

	virtual TypeInfo get_type() const = 0;

	virtual int get_num_args() const = 0;
	virtual const char *get_args() const = 0;

	virtual String get_info(const char *name) const = 0;

	virtual Variable run(void *object, void *i, Variable *args) const = 0;
};

/*
 */
template <class Class, class TList, class Func> class ExternClassFunction: public ExternClassFunctionBase
{
public:
	ExternClassFunction(const char *name, Func func, const char *args): func(func), args(args), name(name) {}

private:
	enum { Length = Length<TList>::Value - 1 };

	typedef typename TypeAt<TList,  0>::Type Ret;
	typedef typename TypeAt<TList,  1>::Type A0;
	typedef typename TypeAt<TList,  2>::Type A1;
	typedef typename TypeAt<TList,  3>::Type A2;
	typedef typename TypeAt<TList,  4>::Type A3;
	typedef typename TypeAt<TList,  5>::Type A4;
	typedef typename TypeAt<TList,  6>::Type A5;
	typedef typename TypeAt<TList,  7>::Type A6;
	typedef typename TypeAt<TList,  8>::Type A7;
	typedef typename TypeAt<TList,  9>::Type A8;
	typedef typename TypeAt<TList, 10>::Type A9;

	virtual const char *get_name() const
	{
		return name.get();
	}

	virtual TypeInfo get_type() const
	{
		return TypeInfo(TypeID<Ret>());
	}

	virtual int get_num_args() const
	{
		return Length;
	}
	virtual const char *get_args() const
	{
		return args.get();
	}

	virtual String get_info(const char *class_name) const
	{
		#ifdef UNIGINE_EXTERN_INFO
			return do_get_info(class_name, IntToType<Length>());
		#else
			return String();
		#endif
	}

	#ifdef UNIGINE_EXTERN_INFO
		String do_get_info(const char *class_name, IntToType<0>) const
		{
			return String::format("\t%s %s();\n", TypeToName<Ret>::name(), class_name);
		}
		String do_get_info(const char *class_name, IntToType<1>) const
		{
			return String::format("\t%s %s(%s);\n", TypeToName<Ret>::name(), class_name,
								  TypeToName<A0>::name());
		}
		String do_get_info(const char *name, IntToType<2>) const
		{
			return String::format("\t%s %s(%s,%s);\n", TypeToName<Ret>::name(), name,
								  TypeToName<A0>::name(), TypeToName<A1>::name());
		}
		String do_get_info(const char *name, IntToType<3>) const
		{
			return String::format("\t%s %s(%s,%s,%s);\n", TypeToName<Ret>::name(), name,
								  TypeToName<A0>::name(), TypeToName<A1>::name(), TypeToName<A2>::name());
		}
		String do_get_info(const char *name, IntToType<4>) const
		{
			return String::format("\t%s %s(%s,%s,%s,%s);\n", TypeToName<Ret>::name(), name,
								  TypeToName<A0>::name(), TypeToName<A1>::name(), TypeToName<A2>::name(), TypeToName<A3>::name());
		}
		String do_get_info(const char *name, IntToType<5>) const
		{
			return String::format("\t%s %s(%s,%s,%s,%s,%s);\n", TypeToName<Ret>::name(), name,
								  TypeToName<A0>::name(), TypeToName<A1>::name(), TypeToName<A2>::name(), TypeToName<A3>::name(),
								  TypeToName<A4>::name());
		}
		String do_get_info(const char *name, IntToType<6>) const
		{
			return String::format("\t%s %s(%s,%s,%s,%s,%s,%s);\n", TypeToName<Ret>::name(), name,
								  TypeToName<A0>::name(), TypeToName<A1>::name(), TypeToName<A2>::name(), TypeToName<A3>::name(),
								  TypeToName<A4>::name(), TypeToName<A5>::name());
		}
		String do_get_info(const char *name, IntToType<7>) const
		{
			return String::format("\t%s %s(%s,%s,%s,%s,%s,%s,%s);\n", TypeToName<Ret>::name(), name,
								  TypeToName<A0>::name(), TypeToName<A1>::name(), TypeToName<A2>::name(), TypeToName<A3>::name(),
								  TypeToName<A4>::name(), TypeToName<A5>::name(), TypeToName<A6>::name());
		}
		String do_get_info(const char *name, IntToType<8>) const
		{
			return String::format("\t%s %s(%s,%s,%s,%s,%s,%s,%s,%s);\n", TypeToName<Ret>::name(), name,
								  TypeToName<A0>::name(), TypeToName<A1>::name(), TypeToName<A2>::name(), TypeToName<A3>::name(),
								  TypeToName<A4>::name(), TypeToName<A5>::name(), TypeToName<A6>::name(), TypeToName<A7>::name());
		}
		String do_get_info(const char *name, IntToType<9>) const
		{
			return String::format("\t%s %s(%s,%s,%s,%s,%s,%s,%s,%s,%s);\n", TypeToName<Ret>::name(), name,
								  TypeToName<A0>::name(), TypeToName<A1>::name(), TypeToName<A2>::name(), TypeToName<A3>::name(),
								  TypeToName<A4>::name(), TypeToName<A5>::name(), TypeToName<A6>::name(), TypeToName<A7>::name(),
								  TypeToName<A8>::name());
		}
		String do_get_info(const char *name, IntToType<10>) const
		{
			return String::format("\t%s %s(%s,%s,%s,%s,%s,%s,%s,%s,%s,%s);\n", TypeToName<Ret>::name(), name,
				TypeToName<A0>::name(), TypeToName<A1>::name(), TypeToName<A2>::name(), TypeToName<A3>::name(),
				TypeToName<A4>::name(), TypeToName<A5>::name(), TypeToName<A6>::name(), TypeToName<A7>::name(),
				TypeToName<A8>::name(), TypeToName<A9>::name());
		}
	#endif

	virtual Variable run(void *object, void *i, Variable *arguments) const
	{
		return do_run(object, i, arguments, IntToType<IsClassFunction<Func>::Value>(), TypeToType<Ret>(), IntToType<Length>());
	}
	template <class Type> UNIGINE_INLINE Variable do_run(void *object, void *i, Variable *args_array, IntToType<1>, Type, IntToType<0>) const
	{
		UNIGINE_UNUSED(args_array);
		TypeToVariable<Ret> ret(i, (static_cast<Class *>(object)->*func)());
		return ret.value;
	}
	UNIGINE_INLINE Variable do_run(void *object, void *i, Variable *arguments, IntToType<1>, TypeToType<void>, IntToType<0>) const
	{
		UNIGINE_UNUSED(arguments);
		UNIGINE_UNUSED(i);
		(static_cast<Class *>(object)->*func)();
		return Variable();
	}
	template <class Type> UNIGINE_INLINE Variable do_run(void *object, void *i, Variable *args_array, IntToType<0>, Type, IntToType<0>) const
	{
		UNIGINE_UNUSED(args_array);
		TypeToVariable<Ret> ret(i, func(static_cast<Class *>(object)));
		return ret.value;
	}
	UNIGINE_INLINE Variable do_run(void *object, void *i, Variable *args_array, IntToType<0>, TypeToType<void>, IntToType<0>) const
	{
		UNIGINE_UNUSED(args_array);
		func(static_cast<Class *>(object));
		return Variable();
	}
	template <class Type> UNIGINE_INLINE Variable do_run(void *object, void *i, Variable *args_array, IntToType<1>, Type, IntToType<1>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		TypeToVariable<Ret> ret(i, (static_cast<Class *>(object)->*func)(a0.value));
		return ret.value;
	}
	UNIGINE_INLINE Variable do_run(void *object, void *i, Variable *args_array, IntToType<1>, TypeToType<void>, IntToType<1>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		(static_cast<Class *>(object)->*func)(a0.value);
		return Variable();
	}
	template <class Type> UNIGINE_INLINE Variable do_run(void *object, void *i, Variable *args_array, IntToType<0>, Type, IntToType<1>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		TypeToVariable<Ret> ret(i, func(static_cast<Class *>(object), a0.value));
		return ret.value;
	}
	UNIGINE_INLINE Variable do_run(void *object, void *i, Variable *args_array, IntToType<0>, TypeToType<void>, IntToType<1>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		func(static_cast<Class *>(object), a0.value);
		return Variable();
	}
	template <class Type> UNIGINE_INLINE Variable do_run(void *object, void *i, Variable *args_array, IntToType<1>, Type, IntToType<2>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		TypeToVariable<Ret> ret(i, (static_cast<Class *>(object)->*func)(a0.value, a1.value));
		return ret.value;
	}
	UNIGINE_INLINE Variable do_run(void *object, void *i, Variable *args_array, IntToType<1>, TypeToType<void>, IntToType<2>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		(static_cast<Class *>(object)->*func)(a0.value, a1.value);
		return Variable();
	}
	template <class Type> UNIGINE_INLINE Variable do_run(void *object, void *i, Variable *args_array, IntToType<0>, Type, IntToType<2>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		TypeToVariable<Ret> ret(i, func(static_cast<Class *>(object), a0.value, a1.value));
		return ret.value;
	}
	UNIGINE_INLINE Variable do_run(void *object, void *i, Variable *args_array, IntToType<0>, TypeToType<void>, IntToType<2>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		func(static_cast<Class *>(object), a0.value, a1.value);
		return Variable();
	}
	template <class Type> UNIGINE_INLINE Variable do_run(void *object, void *i, Variable *args_array, IntToType<1>, Type, IntToType<3>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		TypeToVariable<Ret> ret(i, (static_cast<Class *>(object)->*func)(a0.value, a1.value, a2.value));
		return ret.value;
	}
	UNIGINE_INLINE Variable do_run(void *object, void *i, Variable *args_array, IntToType<1>, TypeToType<void>, IntToType<3>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		(static_cast<Class *>(object)->*func)(a0.value, a1.value, a2.value);
		return Variable();
	}
	template <class Type> UNIGINE_INLINE Variable do_run(void *object, void *i, Variable *args_array, IntToType<0>, Type, IntToType<3>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		TypeToVariable<Ret> ret(i, func(static_cast<Class *>(object), a0.value, a1.value, a2.value));
		return ret.value;
	}
	UNIGINE_INLINE Variable do_run(void *object, void *i, Variable *args_array, IntToType<0>, TypeToType<void>, IntToType<3>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		func(static_cast<Class *>(object), a0.value, a1.value, a2.value);
		return Variable();
	}
	template <class Type> UNIGINE_INLINE Variable do_run(void *object, void *i, Variable *args_array, IntToType<1>, Type, IntToType<4>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		TypeToVariable<Ret> ret(i, (static_cast<Class *>(object)->*func)(a0.value, a1.value, a2.value, a3.value));
		return ret.value;
	}
	UNIGINE_INLINE Variable do_run(void *object, void *i, Variable *args_array, IntToType<1>, TypeToType<void>, IntToType<4>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		(static_cast<Class *>(object)->*func)(a0.value, a1.value, a2.value, a3.value);
		return Variable();
	}
	template <class Type> UNIGINE_INLINE Variable do_run(void *object, void *i, Variable *args_array, IntToType<0>, Type, IntToType<4>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		TypeToVariable<Ret> ret(i, func(static_cast<Class *>(object), a0.value, a1.value, a2.value, a3.value));
		return ret.value;
	}
	UNIGINE_INLINE Variable do_run(void *object, void *i, Variable *args_array, IntToType<0>, TypeToType<void>, IntToType<4>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		func(static_cast<Class *>(object), a0.value, a1.value, a2.value, a3.value);
		return Variable();
	}
	template <class Type> UNIGINE_INLINE Variable do_run(void *object, void *i, Variable *args_array, IntToType<1>, Type, IntToType<5>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		TypeToVariable<Ret> ret(i, (static_cast<Class *>(object)->*func)(a0.value, a1.value, a2.value, a3.value, a4.value));
		return ret.value;
	}
	UNIGINE_INLINE Variable do_run(void *object, void *i, Variable *args_array, IntToType<1>, TypeToType<void>, IntToType<5>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		(static_cast<Class *>(object)->*func)(a0.value, a1.value, a2.value, a3.value, a4.value);
		return Variable();
	}
	template <class Type> UNIGINE_INLINE Variable do_run(void *object, void *i, Variable *args_array, IntToType<0>, Type, IntToType<5>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		TypeToVariable<Ret> ret(i, func(static_cast<Class *>(object), a0.value, a1.value, a2.value, a3.value, a4.value));
		return ret.value;
	}
	UNIGINE_INLINE Variable do_run(void *object, void *i, Variable *args_array, IntToType<0>, TypeToType<void>, IntToType<5>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		func(static_cast<Class *>(object), a0.value, a1.value, a2.value, a3.value, a4.value);
		return Variable();
	}
	template <class Type> UNIGINE_INLINE Variable do_run(void *object, void *i, Variable *args_array, IntToType<1>, Type, IntToType<6>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		VariableToType<A5> a5(i, args_array[5]);
		TypeToVariable<Ret> ret(i, (static_cast<Class *>(object)->*func)(a0.value, a1.value, a2.value, a3.value, a4.value, a5.value));
		return ret.value;
	}
	UNIGINE_INLINE Variable do_run(void *object, void *i, Variable *args_array, IntToType<1>, TypeToType<void>, IntToType<6>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		VariableToType<A5> a5(i, args_array[5]);
		(static_cast<Class *>(object)->*func)(a0.value, a1.value, a2.value, a3.value, a4.value, a5.value);
		return Variable();
	}
	template <class Type> UNIGINE_INLINE Variable do_run(void *object, void *i, Variable *args_array, IntToType<0>, Type, IntToType<6>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		VariableToType<A5> a5(i, args_array[5]);
		TypeToVariable<Ret> ret(i, func(static_cast<Class *>(object), a0.value, a1.value, a2.value, a3.value, a4.value, a5.value));
		return ret.value;
	}
	UNIGINE_INLINE Variable do_run(void *object, void *i, Variable *args_array, IntToType<0>, TypeToType<void>, IntToType<6>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		VariableToType<A5> a5(i, args_array[5]);
		func(static_cast<Class *>(object), a0.value, a1.value, a2.value, a3.value, a4.value, a5.value);
		return Variable();
	}
	template <class Type> UNIGINE_INLINE Variable do_run(void *object, void *i, Variable *args_array, IntToType<1>, Type, IntToType<7>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		VariableToType<A5> a5(i, args_array[5]);
		VariableToType<A6> a6(i, args_array[6]);
		TypeToVariable<Ret> ret(i, (static_cast<Class *>(object)->*func)(a0.value, a1.value, a2.value, a3.value, a4.value, a5.value, a6.value));
		return ret.value;
	}
	UNIGINE_INLINE Variable do_run(void *object, void *i, Variable *args_array, IntToType<1>, TypeToType<void>, IntToType<7>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		VariableToType<A5> a5(i, args_array[5]);
		VariableToType<A6> a6(i, args_array[6]);
		(static_cast<Class *>(object)->*func)(a0.value, a1.value, a2.value, a3.value, a4.value, a5.value, a6.value);
		return Variable();
	}
	template <class Type> UNIGINE_INLINE Variable do_run(void *object, void *i, Variable *args_array, IntToType<0>, Type, IntToType<7>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		VariableToType<A5> a5(i, args_array[5]);
		VariableToType<A6> a6(i, args_array[6]);
		TypeToVariable<Ret> ret(i, func(static_cast<Class *>(object), a0.value, a1.value, a2.value, a3.value, a4.value, a5.value, a6.value));
		return ret.value;
	}
	UNIGINE_INLINE Variable do_run(void *object, void *i, Variable *args_array, IntToType<0>, TypeToType<void>, IntToType<7>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		VariableToType<A5> a5(i, args_array[5]);
		VariableToType<A6> a6(i, args_array[6]);
		func(static_cast<Class *>(object), a0.value, a1.value, a2.value, a3.value, a4.value, a5.value, a6.value);
		return Variable();
	}
	template <class Type> UNIGINE_INLINE Variable do_run(void *object, void *i, Variable *args_array, IntToType<1>, Type, IntToType<8>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		VariableToType<A5> a5(i, args_array[5]);
		VariableToType<A6> a6(i, args_array[6]);
		VariableToType<A7> a7(i, args_array[7]);
		TypeToVariable<Ret> ret(i, (static_cast<Class *>(object)->*func)(a0.value, a1.value, a2.value, a3.value, a4.value, a5.value, a6.value, a7.value));
		return ret.value;
	}
	UNIGINE_INLINE Variable do_run(void *object, void *i, Variable *args_array, IntToType<1>, TypeToType<void>, IntToType<8>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		VariableToType<A5> a5(i, args_array[5]);
		VariableToType<A6> a6(i, args_array[6]);
		VariableToType<A7> a7(i, args_array[7]);
		(static_cast<Class *>(object)->*func)(a0.value, a1.value, a2.value, a3.value, a4.value, a5.value, a6.value, a7.value);
		return Variable();
	}
	template <class Type> UNIGINE_INLINE Variable do_run(void *object, void *i, Variable *args_array, IntToType<0>, Type, IntToType<8>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		VariableToType<A5> a5(i, args_array[5]);
		VariableToType<A6> a6(i, args_array[6]);
		VariableToType<A7> a7(i, args_array[7]);
		TypeToVariable<Ret> ret(i, func(static_cast<Class *>(object), a0.value, a1.value, a2.value, a3.value, a4.value, a5.value, a6.value, a7.value));
		return ret.value;
	}
	UNIGINE_INLINE Variable do_run(void *object, void *i, Variable *args_array, IntToType<0>, TypeToType<void>, IntToType<8>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		VariableToType<A5> a5(i, args_array[5]);
		VariableToType<A6> a6(i, args_array[6]);
		VariableToType<A7> a7(i, args_array[7]);
		func(static_cast<Class *>(object), a0.value, a1.value, a2.value, a3.value, a4.value, a5.value, a6.value, a7.value);
		return Variable();
	}
	template <class Type> UNIGINE_INLINE Variable do_run(void *object, void *i, Variable *args_array, IntToType<1>, Type, IntToType<9>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		VariableToType<A5> a5(i, args_array[5]);
		VariableToType<A6> a6(i, args_array[6]);
		VariableToType<A7> a7(i, args_array[7]);
		VariableToType<A8> a8(i, args_array[8]);
		TypeToVariable<Ret> ret(i, (static_cast<Class *>(object)->*func)(a0.value, a1.value, a2.value, a3.value, a4.value, a5.value, a6.value, a7.value, a8.value));
		return ret.value;
	}
	UNIGINE_INLINE Variable do_run(void *object, void *i, Variable *args_array, IntToType<1>, TypeToType<void>, IntToType<9>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		VariableToType<A5> a5(i, args_array[5]);
		VariableToType<A6> a6(i, args_array[6]);
		VariableToType<A7> a7(i, args_array[7]);
		VariableToType<A8> a8(i, args_array[8]);
		(static_cast<Class *>(object)->*func)(a0.value, a1.value, a2.value, a3.value, a4.value, a5.value, a6.value, a7.value, a8.value);
		return Variable();
	}
	template <class Type> UNIGINE_INLINE Variable do_run(void *object, void *i, Variable *args_array, IntToType<0>, Type, IntToType<9>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		VariableToType<A5> a5(i, args_array[5]);
		VariableToType<A6> a6(i, args_array[6]);
		VariableToType<A7> a7(i, args_array[7]);
		VariableToType<A8> a8(i, args_array[8]);
		TypeToVariable<Ret> ret(i, func(static_cast<Class *>(object), a0.value, a1.value, a2.value, a3.value, a4.value, a5.value, a6.value, a7.value, a8.value));
		return ret.value;
	}
	UNIGINE_INLINE Variable do_run(void *object, void *i, Variable *args_array, IntToType<0>, TypeToType<void>, IntToType<9>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		VariableToType<A5> a5(i, args_array[5]);
		VariableToType<A6> a6(i, args_array[6]);
		VariableToType<A7> a7(i, args_array[7]);
		VariableToType<A8> a8(i, args_array[8]);
		func(static_cast<Class *>(object), a0.value, a1.value, a2.value, a3.value, a4.value, a5.value, a6.value, a7.value, a8.value);
		return Variable();
	}
	template <class Type> UNIGINE_INLINE Variable do_run(void *object, void *i, Variable *args_array, IntToType<1>, Type, IntToType<10>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		VariableToType<A5> a5(i, args_array[5]);
		VariableToType<A6> a6(i, args_array[6]);
		VariableToType<A7> a7(i, args_array[7]);
		VariableToType<A8> a8(i, args_array[8]);
		VariableToType<A9> a9(i, args_array[9]);
		TypeToVariable<Ret> ret(i, (static_cast<Class *>(object)->*func)(a0.value, a1.value, a2.value, a3.value, a4.value, a5.value, a6.value, a7.value, a8.value, a9.value));
		return ret.value;
	}
	UNIGINE_INLINE Variable do_run(void *object, void *i, Variable *args_array, IntToType<1>, TypeToType<void>, IntToType<10>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		VariableToType<A5> a5(i, args_array[5]);
		VariableToType<A6> a6(i, args_array[6]);
		VariableToType<A7> a7(i, args_array[7]);
		VariableToType<A8> a8(i, args_array[8]);
		VariableToType<A9> a9(i, args_array[9]);
		(static_cast<Class *>(object)->*func)(a0.value, a1.value, a2.value, a3.value, a4.value, a5.value, a6.value, a7.value, a8.value, a9.value);
		return Variable();
	}
	template <class Type> UNIGINE_INLINE Variable do_run(void *object, void *i, Variable *args_array, IntToType<0>, Type, IntToType<10>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		VariableToType<A5> a5(i, args_array[5]);
		VariableToType<A6> a6(i, args_array[6]);
		VariableToType<A7> a7(i, args_array[7]);
		VariableToType<A8> a8(i, args_array[8]);
		VariableToType<A9> a9(i, args_array[9]);
		TypeToVariable<Ret> ret(i, func(static_cast<Class *>(object), a0.value, a1.value, a2.value, a3.value, a4.value, a5.value, a6.value, a7.value, a8.value, a9.value));
		return ret.value;
	}
	UNIGINE_INLINE Variable do_run(void *object, void *i, Variable *args_array, IntToType<0>, TypeToType<void>, IntToType<10>) const
	{
		VariableToType<A0> a0(i, args_array[0]);
		VariableToType<A1> a1(i, args_array[1]);
		VariableToType<A2> a2(i, args_array[2]);
		VariableToType<A3> a3(i, args_array[3]);
		VariableToType<A4> a4(i, args_array[4]);
		VariableToType<A5> a5(i, args_array[5]);
		VariableToType<A6> a6(i, args_array[6]);
		VariableToType<A7> a7(i, args_array[7]);
		VariableToType<A8> a8(i, args_array[8]);
		VariableToType<A9> a9(i, args_array[9]);
		func(static_cast<Class *>(object), a0.value, a1.value, a2.value, a3.value, a4.value, a5.value, a6.value, a7.value, a8.value, a9.value);
		return Variable();
	}

	Func func;
	String args;
	String name;
};

/*
 */
template <class Class, class Type> class ExternClassSetFunction: public ExternClassFunctionBase
{
public:
	ExternClassSetFunction(const char *name, Type Class:: *member): member(member), name(name) {}

private:

	virtual const char *get_name() const
	{
		return name.get();
	}

	virtual TypeInfo get_type() const
	{
		return TypeInfo(TypeID<void>());
	}

	virtual int get_num_args() const
	{
		return 1;
	}
	virtual const char *get_args() const
	{
		return NULL;
	}

	virtual String get_info(const char *in_name) const
	{
		#ifdef UNIGINE_EXTERN_INFO
			return String::format("\tvoid %s(%s);\n", in_name, TypeToName<Type>::name());
		#else
			return String();
		#endif
	}

	virtual Variable run(void *object, void *i, Variable *args) const
	{
		VariableToType<Type> value(i, args[0]);
		static_cast<Class *>(object)->*member = value.value;
		return Variable();
	}

	Type Class:: *member;
	String name;
};

/*
 */
template <class Class, class Type> class ExternClassGetFunction: public ExternClassFunctionBase
{
public:
	ExternClassGetFunction(const char *name, Type Class:: *member): member(member), name(name) {}

private:
	virtual const char *get_name() const
	{
		return name.get();
	}

	virtual TypeInfo get_type() const
	{
		return TypeInfo(TypeID<Type>());
	}

	virtual int get_num_args() const
	{
		return 0;
	}
	virtual const char *get_args() const
	{
		return NULL;
	}

	virtual String get_info(const char *in_name) const
	{
	#ifdef UNIGINE_EXTERN_INFO
		return String::format("\t%s %s();\n", TypeToName<Type>::name(), in_name);
	#else
		return String();
	#endif
	}

	virtual Variable run(void *object, void *i, Variable *args) const
	{
		TypeToVariable<Type> value(i, static_cast<Class *>(object)->*member);
		return value.value;
	}

	Type Class:: *member;
	String name;
};

/// @endcond

/**************************************************************************\
*
* Extern classes
*
\**************************************************************************/

/// @cond

/*
 */
class UNIGINE_API ExternClassBase
{
public:
	ExternClassBase(): export_class(0) {}
	virtual ~ExternClassBase() {}

private:
	friend class Interpreter;
	friend class ExternClassExport;

	void *get_export();

	static void release_ptr(ExternClassBase *ptr) { delete ptr; }

	virtual String get_info(const char *name) const = 0;

	// managed class
	virtual int is_managed() const = 0;

	// class type
	virtual TypeInfo get_ptr_type_info() const = 0;
	virtual TypeInfo get_ref_type_info() const = 0;
	virtual TypeInfo get_const_ptr_type_info() const = 0;
	virtual TypeInfo get_const_ref_type_info() const = 0;

	// save/restore
	virtual int has_save_restore_state() const = 0;
	virtual void save_object_state(const StreamPtr &stream, void *object) const = 0;
	virtual void *restore_object_state(const StreamPtr &stream) const = 0;
	virtual int has_save_restore_pointer() const = 0;
	virtual void save_object_pointer(const StreamPtr &stream, void *object) const = 0;
	virtual void *restore_object_pointer(const StreamPtr &stream) const = 0;

	// base classes
	virtual int get_num_base_classes() const = 0;
	virtual ExternClassBase *get_base_class(int num) const = 0;

	// constructors
	virtual int get_num_constructors() const = 0;
	virtual int get_num_constructor_args(int num) const = 0;
	virtual const char *get_constructor_args(int num) const = 0;
	virtual void *run_constructor(int num, void *i, Variable *args) const = 0;

	// destructor
	virtual void destructor(void *ptr) const = 0;

	// functions
	virtual int get_num_functions() const = 0;
	virtual const char *get_function_name(int num) const = 0;
	virtual int get_num_function_args(int num) const = 0;
	virtual TypeInfo get_function_type(int num) const = 0;
	virtual const char *get_function_args(int num) const = 0;
	virtual Variable run_function(void *object, int num, void *i, Variable *args) const = 0;

	// memory usage
	virtual size_t get_memory_usage() const = 0;

	void *export_class;
};

/// @endcond

/**
 * Functor for saving the constructed object state.
 *
 * @param stream Write stream for the object serialization.
 * @param object Pointer to the class object.
 */
template <class Class>
void ExternClassSaveState(const StreamPtr &stream, Class *object)
{
	object->saveState(stream);
}

/**
 * Functor for restoring the constructed object state.
 *
 * @param stream Read stream for object serialization.
 * @return Pointer to the class object.
 */
template <class Class>
Class *ExternClassRestoreState(const StreamPtr &stream)
{
	Class *object = new Class();
	object->restoreState(stream);
	return object;
}

/**
 * Functor for saving the external constructed object state.
 *
 * @param stream Write stream for object serialization.
 * @param object Pointer to the class object.
 */
template <class Class>
void ExternClassSavePointer(const StreamPtr &stream, Class *object)
{
	Class::savePointer(stream, object);
}

/**
 * Functor for restoring the external constructed object state.
 *
 * @param stream Read stream for object serialization.
 * @return Pointer to the class object.
 */
template <class Class>
Class *ExternClassRestorePointer(const StreamPtr &stream)
{
	return static_cast<Class *>(Class::restorePointer(stream));
}

/**
 * Functor for deleting the constructed object.
 *
 * @param object Pointer to the class object.
 */
template <class Class>
void ExternClassDestructor(Class *object)
{
	delete object;
}

/**
 * Unigine ExternClass.
 */
template <class Class> class ExternClass: public ExternClassBase
{
public:

	/**
	 * Functor for saving the object state.
	 *
	 * @param stream Write stream for object serialization.
	 * @param object Pointer to the class object.
	 */
	typedef void (*SaveFunc)(const StreamPtr &stream, Class *object);
	/**
	 * Functor for restoring the object state.
	 *
	 * @param stream Read stream for object serialization.
	 * @return Pointer to the class object.
	 */
	typedef Class *(*RestoreFunc)(const StreamPtr &stream);
	/**
	 * Functor for deleting the object.
	 *
	 * @param object Pointer to the class object.
	 */
	typedef void (*DestructorFunc)(Class *object);

	/**
	 * Default constructor.
	 *
	 * @param save_state Functor for saving the constructed object state.
	 * @param restore_state Functor for restoring the constructed object state.
	 * @param save_pointer Functor for saving the external constructed object state.
	 * @param restore_pointer Functor for restoring the external constructed object state.
	 * @param destructor Functor for deleting the constructed object.
	 */
	ExternClass(SaveFunc save_state = 0, RestoreFunc restore_state = 0, SaveFunc save_pointer = 0, RestoreFunc restore_pointer = 0, DestructorFunc destructor = 0, int managed = 0): managed(managed), num_base_classes(0), num_constructors(0), num_functions(0)
	{
		setSaveRestoreState(save_state, restore_state);
		setSaveRestorePointer(save_pointer, restore_pointer);
		setDestructor(destructor);
	}
	/**
	 * Default destructor.
	 */
	~ExternClass()
	{
		for (int i = 0; i < num_constructors; i++)
		{
			delete constructors[i];
		}
		for (int i = 0; i < num_functions; i++)
		{
			delete functions[i];
		}
	}

	/**
	 * Adds a base class to an external class.
	 *
	 * @param base Pointer to the base class.
	 */
	void addBaseClass(ExternClassBase *base)
	{
		for (int i = 0; i < num_base_classes; i++)
		{
			if (base_classes[i] == base) return;
		}
		base_classes[num_base_classes++] = base;
	}

	/**
	 * Adds a constructor to an external class. The constructor can receive up to 9 arguments.
	 *
	 * @param args Default arguments.
	 */
	void addConstructor(const char *args = 0)
	{
		add_constructor(new ExternClassConstructor<Class, typename MakeTypeList<>::Type>(args));
	}
	/**
	 * Adds a constructor to an external class. The constructor can receive up to 9 arguments.
	 *
	 * @param func Functor for creating the object.
	 * @param args Default arguments.
	 */
	void addConstructor(Class *(*func)(), const char *args = 0)
	{
		add_constructor(new ExternClassConstructor<Class, typename MakeTypeList<>::Type, Class *(*)()>(func, args));
	}

	/// @cond

	template <class A0>
	void addConstructor(const char *args = 0)
	{
		add_constructor(new ExternClassConstructor<Class, typename MakeTypeList<A0>::Type>(args));
	}
	template <class A0>
	void addConstructor(Class *(*func)(A0), const char *args = 0)
	{
		add_constructor(new ExternClassConstructor<Class, typename MakeTypeList<A0>::Type, Class *(*)(A0)>(func, args));
	}
	template <class A0, class A1>
	void addConstructor(const char *args = 0)
	{
		add_constructor(new ExternClassConstructor<Class, typename MakeTypeList<A0, A1>::Type>(args));
	}
	template <class A0, class A1>
	void addConstructor(Class *(*func)(A0, A1), const char *args = 0)
	{
		add_constructor(new ExternClassConstructor<Class, typename MakeTypeList<A0, A1>::Type, Class *(*)(A0, A1)>(func, args));
	}
	template <class A0, class A1, class A2>
	void addConstructor(const char *args = 0)
	{
		add_constructor(new ExternClassConstructor<Class, typename MakeTypeList<A0, A1, A2>::Type>(args));
	}
	template <class A0, class A1, class A2>
	void addConstructor(Class *(*func)(A0, A1, A2), const char *args = 0)
	{
		add_constructor(new ExternClassConstructor<Class, typename MakeTypeList<A0, A1, A2>::Type, Class *(*)(A0, A1, A2)>(func, args));
	}
	template <class A0, class A1, class A2, class A3>
	void addConstructor(const char *args = 0)
	{
		add_constructor(new ExternClassConstructor<Class, typename MakeTypeList<A0, A1, A2, A3>::Type>(args));
	}
	template <class A0, class A1, class A2, class A3>
	void addConstructor(Class *(*func)(A0, A1, A2, A3), const char *args = 0)
	{
		add_constructor(new ExternClassConstructor<Class, typename MakeTypeList<A0, A1, A2, A3>::Type, Class *(*)(A0, A1, A2, A3)>(func, args));
	}
	template <class A0, class A1, class A2, class A3, class A4>
	void addConstructor(const char *args = 0)
	{
		add_constructor(new ExternClassConstructor<Class, typename MakeTypeList<A0, A1, A2, A3, A4>::Type>(args));
	}
	template <class A0, class A1, class A2, class A3, class A4>
	void addConstructor(Class *(*func)(A0, A1, A2, A3, A4), const char *args = 0)
	{
		add_constructor(new ExternClassConstructor<Class, typename MakeTypeList<A0, A1, A2, A3, A4>::Type, Class *(*)(A0, A1, A2, A3, A4)>(func, args));
	}
	template <class A0, class A1, class A2, class A3, class A4, class A5>
	void addConstructor(const char *args = 0)
	{
		add_constructor(new ExternClassConstructor<Class, typename MakeTypeList<A0, A1, A2, A3, A4, A5>::Type>(args));
	}
	template <class A0, class A1, class A2, class A3, class A4, class A5>
	void addConstructor(Class *(*func)(A0, A1, A2, A3, A4, A5), const char *args = 0)
	{
		add_constructor(new ExternClassConstructor<Class, typename MakeTypeList<A0, A1, A2, A3, A4, A5>::Type, Class *(*)(A0, A1, A2, A3, A4, A5)>(func, args));
	}
	template <class A0, class A1, class A2, class A3, class A4, class A5, class A6>
	void addConstructor(const char *args = 0)
	{
		add_constructor(new ExternClassConstructor<Class, typename MakeTypeList<A0, A1, A2, A3, A4, A5, A6>::Type>(args));
	}
	template <class A0, class A1, class A2, class A3, class A4, class A5, class A6>
	void addConstructor(Class *(*func)(A0, A1, A2, A3, A4, A5, A6), const char *args = 0)
	{
		add_constructor(new ExternClassConstructor<Class, typename MakeTypeList<A0, A1, A2, A3, A4, A5, A6>::Type, Class *(*)(A0, A1, A2, A3, A4, A5, A6)>(func, args));
	}
	template <class A0, class A1, class A2, class A3, class A4, class A5, class A6, class A7>
	void addConstructor(const char *args = 0)
	{
		add_constructor(new ExternClassConstructor<Class, typename MakeTypeList<A0, A1, A2, A3, A4, A5, A6, A7>::Type>(args));
	}
	template <class A0, class A1, class A2, class A3, class A4, class A5, class A6, class A7>
	void addConstructor(Class *(*func)(A0, A1, A2, A3, A4, A5, A6, A7), const char *args = 0)
	{
		add_constructor(new ExternClassConstructor<Class, typename MakeTypeList<A0, A1, A2, A3, A4, A5, A6, A7>::Type, Class *(*)(A0, A1, A2, A3, A4, A5, A6, A7)>(func, args));
	}
	template <class A0, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
	void addConstructor(const char *args = 0)
	{
		add_constructor(new ExternClassConstructor<Class, typename MakeTypeList<A0, A1, A2, A3, A4, A5, A6, A7, A8>::Type>(args));
	}
	template <class A0, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
	void addConstructor(Class *(*func)(A0, A1, A2, A3, A4, A5, A6, A7, A8), const char *args = 0)
	{
		add_constructor(new ExternClassConstructor<Class, typename MakeTypeList<A0, A1, A2, A3, A4, A5, A6, A7, A8>::Type, Class *(*)(A0, A1, A2, A3, A4, A5, A6, A7, A8)>(func, args));
	}
	template <class A0, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9>
	void addConstructor(const char *args = 0)
	{
		add_constructor(new ExternClassConstructor<Class, typename MakeTypeList<A0, A1, A2, A3, A4, A5, A6, A7, A8, A9>::Type>(args));
	}
	template <class A0, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9>
	void addConstructor(Class *(*func)(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9), const char *args = 0)
	{
		add_constructor(new ExternClassConstructor<Class, typename MakeTypeList<A0, A1, A2, A3, A4, A5, A6, A7, A8, A9>::Type, Class *(*)(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9)>(func, args));
	}

	/// @endcond

	/**
	 * Adds a member function to an external class. The function can receive up to 9 arguments and return void or value of any supported type. Constant member functions are also supported.
	 *
	 * @param name Name of the member function.
	 * @param func Pointer to the member function.
	 * @param args Default arguments.
	 */
	template <class Type, class Ret>
	void addFunction(const char *name, Ret(Type:: *func)(), const char *args = 0)
	{
		add_function(new ExternClassFunction<Class, typename MakeTypeList<Ret>::Type, Ret(Type:: *)()>(name, func, args));
	}

	/// @cond

	template <class Type, class Ret>
	void addFunction(const char *name, Ret(Type:: *func)() const, const char *args = 0)
	{
		add_function(new ExternClassFunction<Class, typename MakeTypeList<Ret>::Type, Ret(Type:: *)() const>(name, func, args));
	}
	template <class Type, class Ret>
	void addFunction(const char *name, Ret(*func)(Type *), const char *args = 0)
	{
		add_function(new ExternClassFunction<Class, typename MakeTypeList<Ret>::Type, Ret(*)(Type *)>(name, func, args));
	}

	template <class Type, class Ret, class A0>
	void addFunction(const char *name, Ret(Type:: *func)(A0), const char *args = 0)
	{
		add_function(new ExternClassFunction<Class, typename MakeTypeList<Ret, A0>::Type, Ret(Type:: *)(A0)>(name, func, args));
	}
	template <class Type, class Ret, class A0>
	void addFunction(const char *name, Ret(Type:: *func)(A0) const, const char *args = 0)
	{
		add_function(new ExternClassFunction<Class, typename MakeTypeList<Ret, A0>::Type, Ret(Type:: *)(A0) const>(name, func, args));
	}
	template <class Type, class Ret, class A0>
	void addFunction(const char *name, Ret(*func)(Type *, A0), const char *args = 0)
	{
		add_function(new ExternClassFunction<Class, typename MakeTypeList<Ret, A0>::Type, Ret(*)(Type *, A0)>(name, func, args));
	}

	template <class Type, class Ret, class A0, class A1>
	void addFunction(const char *name, Ret(Type:: *func)(A0, A1), const char *args = 0)
	{
		add_function(new ExternClassFunction<Class, typename MakeTypeList<Ret, A0, A1>::Type, Ret(Type:: *)(A0, A1)>(name, func, args));
	}
	template <class Type, class Ret, class A0, class A1>
	void addFunction(const char *name, Ret(Type:: *func)(A0, A1) const, const char *args = 0)
	{
		add_function(new ExternClassFunction<Class, typename MakeTypeList<Ret, A0, A1>::Type, Ret(Type:: *)(A0, A1) const>(name, func, args));
	}
	template <class Type, class Ret, class A0, class A1>
	void addFunction(const char *name, Ret(*func)(Type *, A0, A1), const char *args = 0)
	{
		add_function(new ExternClassFunction<Class, typename MakeTypeList<Ret, A0, A1>::Type, Ret(*)(Type *, A0, A1)>(name, func, args));
	}

	template <class Type, class Ret, class A0, class A1, class A2>
	void addFunction(const char *name, Ret(Type:: *func)(A0, A1, A2), const char *args = 0)
	{
		add_function(new ExternClassFunction<Class, typename MakeTypeList<Ret, A0, A1, A2>::Type, Ret(Type:: *)(A0, A1, A2)>(name, func, args));
	}
	template <class Type, class Ret, class A0, class A1, class A2>
	void addFunction(const char *name, Ret(Type:: *func)(A0, A1, A2) const, const char *args = 0)
	{
		add_function(new ExternClassFunction<Class, typename MakeTypeList<Ret, A0, A1, A2>::Type, Ret(Type:: *)(A0, A1, A2) const>(name, func, args));
	}
	template <class Type, class Ret, class A0, class A1, class A2>
	void addFunction(const char *name, Ret(*func)(Type *, A0, A1, A2), const char *args = 0)
	{
		add_function(new ExternClassFunction<Class, typename MakeTypeList<Ret, A0, A1, A2>::Type, Ret(*)(Type *, A0, A1, A2)>(name, func, args));
	}

	template <class Type, class Ret, class A0, class A1, class A2, class A3>
	void addFunction(const char *name, Ret(Type:: *func)(A0, A1, A2, A3), const char *args = 0)
	{
		add_function(new ExternClassFunction<Class, typename MakeTypeList<Ret, A0, A1, A2, A3>::Type, Ret(Type:: *)(A0, A1, A2, A3)>(name, func, args));
	}
	template <class Type, class Ret, class A0, class A1, class A2, class A3>
	void addFunction(const char *name, Ret(Type:: *func)(A0, A1, A2, A3) const, const char *args = 0)
	{
		add_function(new ExternClassFunction<Class, typename MakeTypeList<Ret, A0, A1, A2, A3>::Type, Ret(Type:: *)(A0, A1, A2, A3) const>(name, func, args));
	}
	template <class Type, class Ret, class A0, class A1, class A2, class A3>
	void addFunction(const char *name, Ret(*func)(Type *, A0, A1, A2, A3), const char *args = 0)
	{
		add_function(new ExternClassFunction<Class, typename MakeTypeList<Ret, A0, A1, A2, A3>::Type, Ret(*)(Type *, A0, A1, A2, A3)>(name, func, args));
	}

	template <class Type, class Ret, class A0, class A1, class A2, class A3, class A4>
	void addFunction(const char *name, Ret(Type:: *func)(A0, A1, A2, A3, A4), const char *args = 0)
	{
		add_function(new ExternClassFunction<Class, typename MakeTypeList<Ret, A0, A1, A2, A3, A4>::Type, Ret(Type:: *)(A0, A1, A2, A3, A4)>(name, func, args));
	}
	template <class Type, class Ret, class A0, class A1, class A2, class A3, class A4>
	void addFunction(const char *name, Ret(Type:: *func)(A0, A1, A2, A3, A4) const, const char *args = 0)
	{
		add_function(new ExternClassFunction<Class, typename MakeTypeList<Ret, A0, A1, A2, A3, A4>::Type, Ret(Type:: *)(A0, A1, A2, A3, A4) const>(name, func, args));
	}
	template <class Type, class Ret, class A0, class A1, class A2, class A3, class A4>
	void addFunction(const char *name, Ret(*func)(Type *, A0, A1, A2, A3, A4), const char *args = 0)
	{
		add_function(new ExternClassFunction<Class, typename MakeTypeList<Ret, A0, A1, A2, A3, A4>::Type, Ret(*)(Type *, A0, A1, A2, A3, A4)>(name, func, args));
	}

	template <class Type, class Ret, class A0, class A1, class A2, class A3, class A4, class A5>
	void addFunction(const char *name, Ret(Type:: *func)(A0, A1, A2, A3, A4, A5), const char *args = 0)
	{
		add_function(new ExternClassFunction<Class, typename MakeTypeList<Ret, A0, A1, A2, A3, A4, A5>::Type, Ret(Type:: *)(A0, A1, A2, A3, A4, A5)>(name, func, args));
	}
	template <class Type, class Ret, class A0, class A1, class A2, class A3, class A4, class A5>
	void addFunction(const char *name, Ret(Type:: *func)(A0, A1, A2, A3, A4, A5) const, const char *args = 0)
	{
		add_function(new ExternClassFunction<Class, typename MakeTypeList<Ret, A0, A1, A2, A3, A4, A5>::Type, Ret(Type:: *)(A0, A1, A2, A3, A4, A5) const>(name, func, args));
	}
	template <class Type, class Ret, class A0, class A1, class A2, class A3, class A4, class A5>
	void addFunction(const char *name, Ret(*func)(Type *, A0, A1, A2, A3, A4, A5), const char *args = 0)
	{
		add_function(new ExternClassFunction<Class, typename MakeTypeList<Ret, A0, A1, A2, A3, A4, A5>::Type, Ret(*)(Type *, A0, A1, A2, A3, A4, A5)>(name, func, args));
	}

	template <class Type, class Ret, class A0, class A1, class A2, class A3, class A4, class A5, class A6>
	void addFunction(const char *name, Ret(Type:: *func)(A0, A1, A2, A3, A4, A5, A6), const char *args = 0)
	{
		add_function(new ExternClassFunction<Class, typename MakeTypeList<Ret, A0, A1, A2, A3, A4, A5, A6>::Type, Ret(Type:: *)(A0, A1, A2, A3, A4, A5, A6)>(name, func, args));
	}
	template <class Type, class Ret, class A0, class A1, class A2, class A3, class A4, class A5, class A6>
	void addFunction(const char *name, Ret(Type:: *func)(A0, A1, A2, A3, A4, A5, A6) const, const char *args = 0)
	{
		add_function(new ExternClassFunction<Class, typename MakeTypeList<Ret, A0, A1, A2, A3, A4, A5, A6>::Type, Ret(Type:: *)(A0, A1, A2, A3, A4, A5, A6) const>(name, func, args));
	}
	template <class Type, class Ret, class A0, class A1, class A2, class A3, class A4, class A5, class A6>
	void addFunction(const char *name, Ret(*func)(Type *, A0, A1, A2, A3, A4, A5, A6), const char *args = 0)
	{
		add_function(new ExternClassFunction<Class, typename MakeTypeList<Ret, A0, A1, A2, A3, A4, A5, A6>::Type, Ret(*)(Type *, A0, A1, A2, A3, A4, A5, A6)>(name, func, args));
	}

	template <class Type, class Ret, class A0, class A1, class A2, class A3, class A4, class A5, class A6, class A7>
	void addFunction(const char *name, Ret(Type:: *func)(A0, A1, A2, A3, A4, A5, A6, A7), const char *args = 0)
	{
		add_function(new ExternClassFunction<Class, typename MakeTypeList<Ret, A0, A1, A2, A3, A4, A5, A6, A7>::Type, Ret(Type:: *)(A0, A1, A2, A3, A4, A5, A6, A7)>(name, func, args));
	}
	template <class Type, class Ret, class A0, class A1, class A2, class A3, class A4, class A5, class A6, class A7>
	void addFunction(const char *name, Ret(Type:: *func)(A0, A1, A2, A3, A4, A5, A6, A7) const, const char *args = 0)
	{
		add_function(new ExternClassFunction<Class, typename MakeTypeList<Ret, A0, A1, A2, A3, A4, A5, A6, A7>::Type, Ret(Type:: *)(A0, A1, A2, A3, A4, A5, A6, A7) const>(name, func, args));
	}
	template <class Type, class Ret, class A0, class A1, class A2, class A3, class A4, class A5, class A6, class A7>
	void addFunction(const char *name, Ret(*func)(Type *, A0, A1, A2, A3, A4, A5, A6, A7), const char *args = 0)
	{
		add_function(new ExternClassFunction<Class, typename MakeTypeList<Ret, A0, A1, A2, A3, A4, A5, A6, A7>::Type, Ret(*)(Type *, A0, A1, A2, A3, A4, A5, A6, A7)>(name, func, args));
	}

	template <class Type, class Ret, class A0, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
	void addFunction(const char *name, Ret(Type:: *func)(A0, A1, A2, A3, A4, A5, A6, A7, A8), const char *args = 0)
	{
		add_function(new ExternClassFunction<Class, typename MakeTypeList<Ret, A0, A1, A2, A3, A4, A5, A6, A7, A8>::Type, Ret(Type:: *)(A0, A1, A2, A3, A4, A5, A6, A7, A8)>(name, func, args));
	}
	template <class Type, class Ret, class A0, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
	void addFunction(const char *name, Ret(Type:: *func)(A0, A1, A2, A3, A4, A5, A6, A7, A8) const, const char *args = 0)
	{
		add_function(new ExternClassFunction<Class, typename MakeTypeList<Ret, A0, A1, A2, A3, A4, A5, A6, A7, A8>::Type, Ret(Type:: *)(A0, A1, A2, A3, A4, A5, A6, A7, A8) const>(name, func, args));
	}
	template <class Type, class Ret, class A0, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
	void addFunction(const char *name, Ret(*func)(Type *, A0, A1, A2, A3, A4, A5, A6, A7, A8), const char *args = 0)
	{
		add_function(new ExternClassFunction<Class, typename MakeTypeList<Ret, A0, A1, A2, A3, A4, A5, A6, A7, A8>::Type, Ret(*)(Type *, A0, A1, A2, A3, A4, A5, A6, A7, A8)>(name, func, args));
	}

	template <class Type, class Ret, class A0, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9>
	void addFunction(const char *name, Ret(Type:: *func)(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9), const char *args = 0)
	{
		add_function(new ExternClassFunction<Class, typename MakeTypeList<Ret, A0, A1, A2, A3, A4, A5, A6, A7, A8, A9>::Type, Ret(Type:: *)(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9)>(name, func, args));
	}
	template <class Type, class Ret, class A0, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9>
	void addFunction(const char *name, Ret(Type:: *func)(A0, A1, A2, A3, A4, A5, A6, A7, A8) const, const char *args = 0)
	{
		add_function(new ExternClassFunction<Class, typename MakeTypeList<Ret, A0, A1, A2, A3, A4, A5, A6, A7, A8, A9>::Type, Ret(Type:: *)(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9) const>(name, func, args));
	}
	template <class Type, class Ret, class A0, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9>
	void addFunction(const char *name, Ret(*func)(Type *, A0, A1, A2, A3, A4, A5, A6, A7, A8), const char *args = 0)
	{
		add_function(new ExternClassFunction<Class, typename MakeTypeList<Ret, A0, A1, A2, A3, A4, A5, A6, A7, A8, A9>::Type, Ret(*)(Type *, A0, A1, A2, A3, A4, A5, A6, A7, A8, A9)>(name, func, args));
	}

	/// @endcond

	/**
	 * Adds a setter function to an external class member.
	 *
	 * @param name Name of the setter function.
	 * @param member Class member.
	 */
	template <class Type>
	void addSetFunction(const char *name, Type Class:: *member)
	{
		add_function(new ExternClassSetFunction<Class, Type>(name, member));
	}

	/**
	 * Adds a getter function to an external class member.
	 *
	 * @param name Name of the getter function.
	 * @param member Class member.
	 */
	template <class Type>
	void addGetFunction(const char *name, Type Class:: *member)
	{
		add_function(new ExternClassGetFunction<Class, Type>(name, member));
	}

	/**
	 * Set functors for saving/restoring state.
	 *
	 * @param save Functor for saving the constructed object state.
	 * @param restore Functor for restoring the constructed object state.
	 */
	void setSaveRestoreState(SaveFunc save, RestoreFunc restore)
	{
		if (save != 0 && restore != 0)
		{
			save_state_func = save;
			restore_state_func = restore;
		} else
		{
			save_state_func = 0;
			restore_state_func = 0;
		}
	}
	/**
	 * Set functors for saving/restoring pointers.
	 *
	 * @param save Functor for saving the external constructed object state.
	 * @param restore Functor for restoring the external constructed object state.
	 */
	void setSaveRestorePointer(SaveFunc save, RestoreFunc restore)
	{
		if (save != 0 && restore != 0)
		{
			save_pointer_func = save;
			restore_pointer_func = restore;
		} else
		{
			save_pointer_func = 0;
			restore_pointer_func = 0;
		}
	}
	/**
	 * Set functor for deleting constructed objects.
	 *
	 * @param destructor Functor for deleting the constructed object.
	 */
	void setDestructor(DestructorFunc destructor)
	{
		destructor_func = destructor;
	}

private:

	/// @cond

	enum
	{
		NUM_BASE_CLASSES = 32,
		NUM_CONSTRUCTORS = 32,
		NUM_FUNCTIONS = 128,
	};

	void add_constructor(ExternClassConstructorBase *constructor)
	{
		UNIGINE_ASSERT(num_constructors < NUM_CONSTRUCTORS && "ExternClass::add_constructor(): constructors overflow");
		constructors[num_constructors++] = constructor;
	}
	void add_function(ExternClassFunctionBase *function)
	{
		UNIGINE_ASSERT(num_functions < NUM_FUNCTIONS && "ExternClass::add_function(): functions overflow");
		functions[num_functions++] = function;
	}

	virtual String get_info(const char *name) const
	{
		#ifdef UNIGINE_EXTERN_INFO
			String ret = String::format("class %s {\n", name);
			for (int i = 0; i < num_constructors; i++)
			{
				ret += constructors[i]->get_info(name);
			}
			for (int i = 0; i < num_functions; i++)
			{
				ret += functions[i]->get_info(functions[i]->get_name());
			}
			ret += String::format("};\n");
			return ret;
		#else
			return String();
		#endif
	}

	// managed class
	virtual int is_managed() const
	{
		return managed;
	}

	// class type
	virtual TypeInfo get_ptr_type_info() const
	{
		return TypeInfo(TypeID<Class *>());
	}
	virtual TypeInfo get_ref_type_info() const
	{
		return TypeInfo(TypeID<Class &>());
	}
	virtual TypeInfo get_const_ptr_type_info() const
	{
		return TypeInfo(TypeID<const Class *>());
	}
	virtual TypeInfo get_const_ref_type_info() const
	{
		return TypeInfo(TypeID<const Class &>());
	}

	// save/restore
	virtual int has_save_restore_state() const
	{
		return (save_state_func != 0 && restore_state_func != 0);
	}
	virtual void save_object_state(const StreamPtr &stream, void *object) const
	{
		save_state_func(stream, static_cast<Class *>(object));
	}
	virtual void *restore_object_state(const StreamPtr &stream) const
	{
		return restore_state_func(stream);
	}
	virtual int has_save_restore_pointer() const
	{
		return (save_pointer_func != 0 && restore_pointer_func != 0);
	}
	virtual void save_object_pointer(const StreamPtr &stream, void *object) const
	{
		save_pointer_func(stream, static_cast<Class *>(object));
	}
	virtual void *restore_object_pointer(const StreamPtr &stream) const
	{
		return restore_pointer_func(stream);
	}

	// base classes
	virtual int get_num_base_classes() const
	{
		return num_base_classes;
	}
	virtual ExternClassBase *get_base_class(int num) const
	{
		return base_classes[num];
	}

	// constructors
	virtual int get_num_constructors() const
	{
		return num_constructors;
	}
	virtual int get_num_constructor_args(int num) const
	{
		return constructors[num]->get_num_args();
	}
	virtual const char *get_constructor_args(int num) const
	{
		return constructors[num]->get_args();
	}
	virtual void *run_constructor(int num, void *i, Variable *args) const
	{
		return constructors[num]->run(i, args);
	}

	// destructor
	virtual void destructor(void *object) const
	{
		if (destructor_func != 0) destructor_func(static_cast<Class *>(object));
		else delete static_cast<Class *>(object);
	}

	// functions
	virtual int get_num_functions() const
	{
		return num_functions;
	}
	virtual const char *get_function_name(int num) const
	{
		return functions[num]->get_name();
	}
	virtual TypeInfo get_function_type(int num) const
	{
		return functions[num]->get_type();
	}
	virtual int get_num_function_args(int num) const
	{
		return functions[num]->get_num_args();
	}
	virtual const char *get_function_args(int num) const
	{
		return functions[num]->get_args();
	}
	virtual Variable run_function(void *object, int num, void *i, Variable *args) const
	{
		return functions[num]->run(object, i, args);
	}

	// memory usage
	virtual size_t get_memory_usage() const
	{
		return sizeof(Class);
	}

	int managed;

	int num_base_classes;
	ExternClassBase *base_classes[NUM_BASE_CLASSES];

	int num_constructors;
	ExternClassConstructorBase *constructors[NUM_CONSTRUCTORS];

	int num_functions;
	ExternClassFunctionBase *functions[NUM_FUNCTIONS];

	SaveFunc save_state_func;
	RestoreFunc restore_state_func;
	SaveFunc save_pointer_func;
	RestoreFunc restore_pointer_func;
	DestructorFunc destructor_func;

	/// @endcond
};

/**
 * Make a pointer to an external class.
 *
 * @return Pointer to the external class.
 */
template <class Class>
ExternClass<Class> *MakeExternClass()
{
	return new ExternClass<Class>();
}

/**
 * Make a pointer to an external managed class.
 */
template <class Class>
ExternClass<Class> *MakeExternClassManaged()
{
	return new ExternClass<Class>(0, 0, 0, 0, 0, 1);
}

/**
 * Make a pointer to an external class with functions for saving/restoring the constructed object state.
 *
 * @param save Functor for saving the constructed object state.
 * @param restore Functor for restoring the constructed object state.
 * @return Pointer to the external class.
 */
template <class Class>
ExternClass<Class> *MakeExternClassSaveRestoreState(typename ExternClass<Class>::SaveFunc save = ExternClassSaveState<Class>, typename ExternClass<Class>::RestoreFunc restore = ExternClassRestoreState<Class>)
{
	return new ExternClass<Class>(save, restore);
}

/**
 * Make a pointer to an external class with functions for saving/restoring the external constructed object state.
 *
 * @param save Functor for saving the constructed object state.
 * @param restore Functor for restoring the constructed object state.
 * @return Pointer to the external class.
 */
template <class Class>
ExternClass<Class> *MakeExternClassSaveRestorePointer(typename ExternClass<Class>::SaveFunc save = ExternClassSavePointer<Class>, typename ExternClass<Class>::RestoreFunc restore = ExternClassRestorePointer<Class>)
{
	return new ExternClass<Class>(0, 0, save, restore);
}

/**
 * Make a pointer to an external class with functions for saving/restoring the object state.
 *
 * @param save_state Functor for saving the constructed object state.
 * @param restore_state Functor for restoring the constructed object state.
 * @param save_pointer Functor for saving the external constructed object state.
 * @param restore_pointer Functor for restoring the external constructed object state.
 */
template <class Class>
ExternClass<Class> *MakeExternClassSaveRestoreStatePointer(typename ExternClass<Class>::SaveFunc save_state = ExternClassSaveState<Class>, typename ExternClass<Class>::RestoreFunc restore_state = ExternClassRestoreState<Class>, typename ExternClass<Class>::SaveFunc save_pointer = ExternClassSavePointer<Class>, typename ExternClass<Class>::RestoreFunc restore_pointer = ExternClassRestorePointer<Class>)
{
	return new ExternClass<Class>(save_state, restore_state, save_pointer, restore_pointer);
}

/**
 * Make a pointer to an external class with functions for deleting the object.
 *
 * @param destructor Functor for deleting the constructed object.
 */
template <class Class>
ExternClass<Class> *MakeExternClassDestructor(typename ExternClass<Class>::DestructorFunc destructor = ExternClassDestructor<Class>)
{
	return new ExternClass<Class>(0, 0, 0, 0, destructor);
}

} /* namespace Unigine */
