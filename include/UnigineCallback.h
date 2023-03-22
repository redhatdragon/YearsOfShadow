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

#include "UnigineBase.h"
#include <type_traits>

namespace Unigine
{

////////////////////////////////////////////////////////////////////////////////
// Base callback class.
////////////////////////////////////////////////////////////////////////////////
class CallbackBase
{
public:
	virtual ~CallbackBase() {}

	virtual void run() = 0;

	template <class A0>
	void run(A0 a0);

	template <class A0, class A1>
	void run(A0 a0, A1 a1);

	template <class A0, class A1, class A2>
	void run(A0 a0, A1 a1, A2 a2);

	template <class A0, class A1, class A2, class A3>
	void run(A0 a0, A1 a1, A2 a2, A3 a3);

	template <class A0, class A1, class A2, class A3, class A4>
	void run(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4);

private:
	virtual int get_arity() const { return 0; }
};

////////////////////////////////////////////////////////////////////////////////
template <class A0>
class CallbackBase1 : public CallbackBase
{
public:
	void run() override { UNIGINE_ASSERT(0 && "CallbackBase1::run(): called"); }
	virtual void run(A0 a0) = 0;

private:
	int get_arity() const override { return 1; }
};

template <class A0, class A1>
class CallbackBase2 : public CallbackBase
{
public:
	void run()  override { UNIGINE_ASSERT(0 && "CallbackBase2::run(): called"); }
	virtual void run(A0 a0, A1 a1) = 0;

private:
	int get_arity() const override { return 2; }
};

template <class A0, class A1, class A2>
class CallbackBase3 : public CallbackBase
{
public:
	void run() override { UNIGINE_ASSERT(0 && "CallbackBase3::run(): called"); }
	virtual void run(A0 a0, A1 a1, A2 a2) = 0;

private:
	int get_arity() const override { return 3; }
};

template <class A0, class A1, class A2, class A3>
class CallbackBase4 : public CallbackBase
{
public:
	void run() override { UNIGINE_ASSERT(0 && "CallbackBase4::run(): called"); }
	virtual void run(A0 a0, A1 a1, A2 a2, A3 a3) = 0;

private:
	int get_arity() const override { return 4; }
};

template <class A0, class A1, class A2, class A3, class A4>
class CallbackBase5 : public CallbackBase
{
public:
	void run() override { UNIGINE_ASSERT(0 && "CallbackBase5::run(): called"); }
	virtual void run(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) = 0;

private:
	int get_arity() const override { return 5; }
};

////////////////////////////////////////////////////////////////////////////////
template <class A0>
void CallbackBase::run(A0 a0)
{
	switch (get_arity())
	{
		case 0: run(); break;
		case 1: static_cast<CallbackBase1<A0> *>(this)->run(a0); break;
		default: UNIGINE_ASSERT(0 && "Unigine::CallbackBase::run(): wrong size");
	}
}

template <class A0, class A1>
void CallbackBase::run(A0 a0, A1 a1)
{
	switch (get_arity())
	{
		case 0: run(); break;
		case 1: static_cast<CallbackBase1<A0> *>(this)->run(a0); break;
		case 2: static_cast<CallbackBase2<A0, A1> *>(this)->run(a0, a1); break;
		default: UNIGINE_ASSERT(0 && "Unigine::CallbackBase::run(): wrong size");
	}
}

template <class A0, class A1, class A2>
void CallbackBase::run(A0 a0, A1 a1, A2 a2)
{
	switch (get_arity())
	{
		case 0: run(); break;
		case 1: static_cast<CallbackBase1<A0> *>(this)->run(a0); break;
		case 2: static_cast<CallbackBase2<A0, A1> *>(this)->run(a0, a1); break;
		case 3: static_cast<CallbackBase3<A0, A1, A2> *>(this)->run(a0, a1, a2); break;
		default: UNIGINE_ASSERT(0 && "Unigine::CallbackBase::run(): wrong size");
	}
}

template <class A0, class A1, class A2, class A3>
void CallbackBase::run(A0 a0, A1 a1, A2 a2, A3 a3)
{
	switch (get_arity())
	{
		case 0: run(); break;
		case 1: static_cast<CallbackBase1<A0> *>(this)->run(a0); break;
		case 2: static_cast<CallbackBase2<A0, A1> *>(this)->run(a0, a1); break;
		case 3: static_cast<CallbackBase3<A0, A1, A2> *>(this)->run(a0, a1, a2); break;
		case 4: static_cast<CallbackBase4<A0, A1, A2, A3> *>(this)->run(a0, a1, a2, a3); break;
		default: UNIGINE_ASSERT(0 && "Unigine::CallbackBase::run(): wrong size");
	}
}

template <class A0, class A1, class A2, class A3, class A4>
void CallbackBase::run(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4)
{
	switch (get_arity())
	{
		case 0: run(); break;
		case 1: static_cast<CallbackBase1<A0> *>(this)->run(a0); break;
		case 2: static_cast<CallbackBase2<A0, A1> *>(this)->run(a0, a1); break;
		case 3: static_cast<CallbackBase3<A0, A1, A2> *>(this)->run(a0, a1, a2); break;
		case 4: static_cast<CallbackBase4<A0, A1, A2, A3> *>(this)->run(a0, a1, a2, a3); break;
		case 5: static_cast<CallbackBase5<A0, A1, A2, A3, A4> *>(this)->run(a0, a1, a2, a3, a4); break;
		default: UNIGINE_ASSERT(0 && "Unigine::CallbackBase::run(): wrong size");
	}
}

////////////////////////////////////////////////////////////////////////////////
/// Free function callbacks.
////////////////////////////////////////////////////////////////////////////////
template <class Func>
class Callback0 : public CallbackBase
{
public:
	Callback0(Func func)
		: func(func)
	{}

	virtual void run() { func(); }

private:
	Func func;
};

template <class Base, class Func, class A0>
class Callback1 : public Base
{
public:
	Callback1(Func func)
		: func(func)
	{}
	Callback1(Func func, A0 a0)
		: func(func), a0(a0)
	{}

	virtual void run() { func(a0); }
	virtual void run(A0 arg0) { func(arg0); }

private:
	Func func;
	typename std::decay<A0>::type a0;
};

template <class Base, class Func, class A0, class A1>
class Callback2 : public Base
{
public:
	Callback2(Func func)
		: func(func)
	{}
	Callback2(Func func, A1 a1)
		: func(func), a1(a1)
	{}
	Callback2(Func func, A0 a0, A1 a1)
		: func(func), a0(a0), a1(a1)
	{}

	virtual void run() { func(a0, a1); }
	virtual void run(A0 arg0) { func(arg0, a1); }
	virtual void run(A0 arg0, A1 arg1) { func(arg0, arg1); }

private:
	Func func;
	typename std::decay<A0>::type a0;
	typename std::decay<A1>::type a1;
};

template <class Base, class Func, class A0, class A1, class A2>
class Callback3 : public Base
{
public:
	Callback3(Func func)
		: func(func)
	{}
	Callback3(Func func, A2 a2)
		: func(func), a2(a2)
	{}
	Callback3(Func func, A1 a1, A2 a2)
		: func(func), a1(a1), a2(a2)
	{}
	Callback3(Func func, A0 a0, A1 a1, A2 a2)
		: func(func), a0(a0), a1(a1), a2(a2)
	{}

	virtual void run() { func(a0, a1, a2); }
	virtual void run(A0 arg0) { func(arg0, a1, a2); }
	virtual void run(A0 arg0, A1 arg1) { func(arg0, arg1, a2); }
	virtual void run(A0 arg0, A1 arg1, A2 arg2) { func(arg0, arg1, arg2); }

private:
	Func func;
	typename std::decay<A0>::type a0;
	typename std::decay<A1>::type a1;
	typename std::decay<A2>::type a2;
};

template <class Base, class Func, class A0, class A1, class A2, class A3>
class Callback4 : public Base
{
public:
	Callback4(Func func)
		: func(func)
	{}
	Callback4(Func func, A3 a3)
		: func(func), a3(a3)
	{}
	Callback4(Func func, A2 a2, A3 a3)
		: func(func), a2(a2), a3(a3)
	{}
	Callback4(Func func, A1 a1, A2 a2, A3 a3)
		: func(func), a1(a1), a2(a2), a3(a3)
	{}
	Callback4(Func func, A0 a0, A1 a1, A2 a2, A3 a3)
		: func(func), a0(a0), a1(a1), a2(a2), a3(a3)
	{}

	virtual void run() { func(a0, a1, a2, a3); }
	virtual void run(A0 arg0) { func(arg0, a1, a2, a3); }
	virtual void run(A0 arg0, A1 arg1) { func(arg0, arg1, a2, a3); }
	virtual void run(A0 arg0, A1 arg1, A2 arg2) { func(arg0, arg1, arg2, a3); }
	virtual void run(A0 arg0, A1 arg1, A2 arg2, A3 arg3) { func(arg0, arg1, arg2, arg3); }

private:
	Func func;
	typename std::decay<A0>::type a0;
	typename std::decay<A1>::type a1;
	typename std::decay<A2>::type a2;
	typename std::decay<A3>::type a3;
};

template <class Base, class Func, class A0, class A1, class A2, class A3, class A4>
class Callback5 : public Base
{
public:
	Callback5(Func func)
		: func(func)
	{}
	Callback5(Func func, A3 a3)
		: func(func), a3(a3)
	{}
	Callback5(Func func, A2 a2, A3 a3)
		: func(func), a2(a2), a3(a3)
	{}
	Callback5(Func func, A1 a1, A2 a2, A3 a3)
		: func(func), a1(a1), a2(a2), a3(a3)
	{}
	Callback5(Func func, A0 a0, A1 a1, A2 a2, A3 a3)
		: func(func), a0(a0), a1(a1), a2(a2), a3(a3)
	{}
	Callback5(Func func, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4)
		: func(func), a0(a0), a1(a1), a2(a2), a3(a3), a4(a4)
	{}

	virtual void run() { func(a0, a1, a2, a3, a4); }
	virtual void run(A0 arg0) { func(arg0, a1, a2, a3, a4); }
	virtual void run(A0 arg0, A1 arg1) { func(arg0, arg1, a2, a3, a4); }
	virtual void run(A0 arg0, A1 arg1, A2 arg2) { func(arg0, arg1, arg2, a3, a4); }
	virtual void run(A0 arg0, A1 arg1, A2 arg2, A3 arg3) { func(arg0, arg1, arg2, arg3, a4); }
	virtual void run(A0 arg0, A1 arg1, A2 arg2, A3 arg3, A4 arg4) { func(arg0, arg1, arg2, arg3, arg4); }

private:
	Func func;
	typename std::decay<A0>::type a0;
	typename std::decay<A1>::type a1;
	typename std::decay<A2>::type a2;
	typename std::decay<A3>::type a3;
	typename std::decay<A4>::type a4;
};

////////////////////////////////////////////////////////////////////////////////
/// Free function callback factories.
////////////////////////////////////////////////////////////////////////////////
template <class Ret>
CallbackBase *MakeCallback(Ret (*func)())
{
	return new Callback0<Ret (*)()>(func);
}

template <class Ret, class A0>
CallbackBase1<A0> *MakeCallback(Ret (*func)(A0))
{
	return new Callback1<CallbackBase1<A0>, Ret (*)(A0), A0>(func);
}

template <class Ret, class A0>
CallbackBase *MakeCallback(Ret (*func)(A0), A0 a0)
{
	return new Callback1<CallbackBase, Ret (*)(A0), A0>(func, a0);
}

template <class Ret, class A0, class A1>
CallbackBase2<A0, A1> *MakeCallback(Ret (*func)(A0, A1))
{
	return new Callback2<CallbackBase2<A0, A1>, Ret (*)(A0, A1), A0, A1>(func);
}

template <class Ret, class A0, class A1>
CallbackBase1<A0> *MakeCallback(Ret (*func)(A0, A1), A1 a1)
{
	return new Callback2<CallbackBase1<A0>, Ret (*)(A0, A1), A0, A1>(func, a1);
}

template <class Ret, class A0, class A1>
CallbackBase *MakeCallback(Ret (*func)(A0, A1), A0 a0, A1 a1)
{
	return new Callback2<CallbackBase, Ret (*)(A0, A1), A0, A1>(func, a0, a1);
}

template <class Ret, class A0, class A1, class A2>
CallbackBase3<A0, A1, A2> *MakeCallback(Ret (*func)(A0, A1, A2))
{
	return new Callback3<CallbackBase3<A0, A1, A2>, Ret (*)(A0, A1, A2), A0, A1, A2>(func);
}

template <class Ret, class A0, class A1, class A2>
CallbackBase2<A0, A1> *MakeCallback(Ret (*func)(A0, A1, A2), A2 a2)
{
	return new Callback3<CallbackBase2<A0, A1>, Ret (*)(A0, A1, A2), A0, A1, A2>(func, a2);
}

template <class Ret, class A0, class A1, class A2>
CallbackBase1<A0> *MakeCallback(Ret (*func)(A0, A1, A2), A1 a1, A2 a2)
{
	return new Callback3<CallbackBase1<A0>, Ret (*)(A0, A1, A2), A0, A1, A2>(func, a1, a2);
}

template <class Ret, class A0, class A1, class A2>
CallbackBase *MakeCallback(Ret (*func)(A0, A1, A2), A0 a0, A1 a1, A2 a2)
{
	return new Callback3<CallbackBase, Ret (*)(A0, A1, A2), A0, A1, A2>(func, a0, a1, a2);
}

template <class Ret, class A0, class A1, class A2, class A3>
CallbackBase4<A0, A1, A2, A3> *MakeCallback(Ret (*func)(A0, A1, A2, A3))
{
	return new Callback4<CallbackBase4<A0, A1, A2, A3>, Ret (*)(A0, A1, A2, A3), A0, A1, A2, A3>(func);
}

template <class Ret, class A0, class A1, class A2, class A3>
CallbackBase3<A0, A1, A2> *MakeCallback(Ret (*func)(A0, A1, A2, A3), A3 a3)
{
	return new Callback4<CallbackBase3<A0, A1, A2>, Ret (*)(A0, A1, A2, A3), A0, A1, A2, A3>(func, a3);
}

template <class Ret, class A0, class A1, class A2, class A3>
CallbackBase2<A0, A1> *MakeCallback(Ret (*func)(A0, A1, A2, A3), A2 a2, A3 a3)
{
	return new Callback4<CallbackBase2<A0, A1>, Ret (*)(A0, A1, A2, A3), A0, A1, A2, A3>(func, a2, a3);
}

template <class Ret, class A0, class A1, class A2, class A3>
CallbackBase1<A0> *MakeCallback(Ret (*func)(A0, A1, A2, A3), A1 a1, A2 a2, A3 a3)
{
	return new Callback4<CallbackBase1<A0>, Ret (*)(A0, A1, A2, A3), A0, A1, A2, A3>(func, a1, a2, a3);
}

template <class Ret, class A0, class A1, class A2, class A3>
CallbackBase *MakeCallback(Ret (*func)(A0, A1, A2, A3), A0 a0, A1 a1, A2 a2, A3 a3)
{
	return new Callback4<CallbackBase, Ret (*)(A0, A1, A2, A3), A0, A1, A2, A3>(func, a0, a1, a2, a3);
}

template <class Ret, class A0, class A1, class A2, class A3, class A4>
CallbackBase5<A0, A1, A2, A3, A4> *MakeCallback(Ret (*func)(A0, A1, A2, A3, A4))
{
	return new Callback5<CallbackBase5<A0, A1, A2, A3, A4>, Ret (*)(A0, A1, A2, A3, A4), A0, A1, A2, A3, A4>(func);
}

template <class Ret, class A0, class A1, class A2, class A3, class A4>
CallbackBase4<A0, A1, A2, A3> *MakeCallback(Ret (*func)(A0, A1, A2, A3, A4), A4 a4)
{
	return new Callback5<CallbackBase4<A0, A1, A2, A3>, Ret (*)(A0, A1, A2, A3, A4), A0, A1, A2, A3, A4>(func, a4);
}

template <class Ret, class A0, class A1, class A2, class A3, class A4>
CallbackBase3<A0, A1, A2> *MakeCallback(Ret (*func)(A0, A1, A2, A3, A4), A3 a3, A4 a4)
{
	return new Callback5<CallbackBase3<A0, A1, A2>, Ret (*)(A0, A1, A2, A3, A4), A0, A1, A2, A3, A4>(func, a3, a4);
}

template <class Ret, class A0, class A1, class A2, class A3, class A4>
CallbackBase2<A0, A1> *MakeCallback(Ret (*func)(A0, A1, A2, A3, A4), A2 a2, A3 a3, A4 a4)
{
	return new Callback5<CallbackBase2<A0, A1>, Ret (*)(A0, A1, A2, A3, A4), A0, A1, A2, A3, A4>(func, a2, a3, a4);
}

template <class Ret, class A0, class A1, class A2, class A3, class A4>
CallbackBase1<A0> *MakeCallback(Ret (*func)(A0, A1, A2, A3, A4), A1 a1, A2 a2, A3 a3, A4 a4)
{
	return new Callback5<CallbackBase1<A0>, Ret (*)(A0, A1, A2, A3, A4), A0, A1, A2, A3, A4>(func, a1, a2, a3, a4);
}

template <class Ret, class A0, class A1, class A2, class A3, class A4>
CallbackBase *MakeCallback(Ret (*func)(A0, A1, A2, A3, A4), A0 a0, A1 a1, A2 a2, A3 a3, A4 a4)
{
	return new Callback5<CallbackBase, Ret (*)(A0, A1, A2, A3, A4), A0, A1, A2, A3, A4>(func, a0, a1, a2, a3, a4);
}

////////////////////////////////////////////////////////////////////////////////
/// Member function callbacks.
////////////////////////////////////////////////////////////////////////////////
template <class Class, class Func>
class CallbackObject0 : public CallbackBase
{
public:
	CallbackObject0(Class *object, Func func)
		: object(object), func(func)
	{}

	virtual void run() { (object->*func)(); }

private:
	Class *object;
	Func func;
};

template <class Base, class Class, class Func, class A0>
class CallbackObject1 : public Base
{
public:
	CallbackObject1(Class *object, Func func)
		: object(object), func(func)
	{}
	CallbackObject1(Class *object, Func func, A0 a0)
		: object(object), func(func), a0(a0)
	{}

	virtual void run() { (object->*func)(a0); }
	virtual void run(A0 arg0) { (object->*func)(arg0); }

private:
	Class *object;
	Func func;
	typename std::decay<A0>::type a0;
};

template <class Base, class Class, class Func, class A0, class A1>
class CallbackObject2 : public Base
{
public:
	CallbackObject2(Class *object, Func func)
		: object(object), func(func)
	{}
	CallbackObject2(Class *object, Func func, A1 a1)
		: object(object), func(func), a1(a1)
	{}
	CallbackObject2(Class *object, Func func, A0 a0, A1 a1)
		: object(object), func(func), a0(a0), a1(a1)
	{}

	virtual void run() { (object->*func)(a0, a1); }
	virtual void run(A0 arg0) { (object->*func)(arg0, a1); }
	virtual void run(A0 arg0, A1 arg1) { (object->*func)(arg0, arg1); }

private:
	Class *object;
	Func func;
	typename std::decay<A0>::type a0;
	typename std::decay<A1>::type a1;
};

template <class Base, class Class, class Func, class A0, class A1, class A2>
class CallbackObject3 : public Base
{
public:
	CallbackObject3(Class *object, Func func)
		: object(object), func(func)
	{}
	CallbackObject3(Class *object, Func func, A2 a2)
		: object(object), func(func), a2(a2)
	{}
	CallbackObject3(Class *object, Func func, A1 a1, A2 a2)
		: object(object), func(func), a1(a1), a2(a2)
	{}
	CallbackObject3(Class *object, Func func, A0 a0, A1 a1, A2 a2)
		: object(object), func(func), a0(a0), a1(a1), a2(a2)
	{}

	virtual void run() { (object->*func)(a0, a1, a2); }
	virtual void run(A0 arg0) { (object->*func)(arg0, a1, a2); }
	virtual void run(A0 arg0, A1 arg1) { (object->*func)(arg0, arg1, a2); }
	virtual void run(A0 arg0, A1 arg1, A2 arg2) { (object->*func)(arg0, arg1, arg2); }

private:
	Class *object;
	Func func;
	typename std::decay<A0>::type a0;
	typename std::decay<A1>::type a1;
	typename std::decay<A2>::type a2;
};

template <class Base, class Class, class Func, class A0, class A1, class A2, class A3>
class CallbackObject4 : public Base
{
public:
	CallbackObject4(Class *object, Func func)
		: object(object), func(func)
	{}
	CallbackObject4(Class *object, Func func, A3 a3)
		: object(object), func(func), a3(a3)
	{}
	CallbackObject4(Class *object, Func func, A2 a2, A3 a3)
		: object(object), func(func), a2(a2), a3(a3)
	{}
	CallbackObject4(Class *object, Func func, A1 a1, A2 a2, A3 a3)
		: object(object), func(func), a1(a1), a2(a2), a3(a3)
	{}
	CallbackObject4(Class *object, Func func, A0 a0, A1 a1, A2 a2, A3 a3)
		: object(object), func(func), a0(a0), a1(a1), a2(a2), a3(a3)
	{}

	virtual void run() { (object->*func)(a0, a1, a2, a3); }
	virtual void run(A0 arg0) { (object->*func)(arg0, a1, a2, a3); }
	virtual void run(A0 arg0, A1 arg1) { (object->*func)(arg0, arg1, a2, a3); }
	virtual void run(A0 arg0, A1 arg1, A2 arg2) { (object->*func)(arg0, arg1, arg2, a3); }
	virtual void run(A0 arg0, A1 arg1, A2 arg2, A3 arg3) { (object->*func)(arg0, arg1, arg2, arg3); }
private:
	Class *object;
	Func func;
	typename std::decay<A0>::type a0;
	typename std::decay<A1>::type a1;
	typename std::decay<A2>::type a2;
	typename std::decay<A3>::type a3;
};

template <class Base, class Class, class Func, class A0, class A1, class A2, class A3, class A4>
class CallbackObject5 : public Base
{
public:
	CallbackObject5(Class *object, Func func)
		: object(object), func(func)
	{}
	CallbackObject5(Class *object, Func func, A4 a4)
		: object(object), func(func), a4(a4)
	{}
	CallbackObject5(Class *object, Func func, A3 a3, A4 a4)
		: object(object), func(func), a3(a3), a4(a4)
	{}
	CallbackObject5(Class *object, Func func, A2 a2, A3 a3, A4 a4)
		: object(object), func(func), a2(a2), a3(a3), a4(a4)
	{}
	CallbackObject5(Class *object, Func func, A1 a1, A2 a2, A3 a3, A4 a4)
		: object(object), func(func), a1(a1), a2(a2), a3(a3), a4(a4)
	{}
	CallbackObject5(Class *object, Func func, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4)
		: object(object), func(func), a0(a0), a1(a1), a2(a2), a3(a3), a4(a4)
	{}

	virtual void run() { (object->*func)(a0, a1, a2, a3, a4); }
	virtual void run(A0 arg0) { (object->*func)(arg0, a1, a2, a3, a4); }
	virtual void run(A0 arg0, A1 arg1) { (object->*func)(arg0, arg1, a2, a3, a4); }
	virtual void run(A0 arg0, A1 arg1, A2 arg2) { (object->*func)(arg0, arg1, arg2, a3, a4); }
	virtual void run(A0 arg0, A1 arg1, A2 arg2, A3 arg3) { (object->*func)(arg0, arg1, arg2, arg3, a4); }
	virtual void run(A0 arg0, A1 arg1, A2 arg2, A3 arg3, A4 arg4) { (object->*func)(arg0, arg1, arg2, arg3, arg4); }

private:
	Class *object;
	Func func;
	typename std::decay<A0>::type a0;
	typename std::decay<A1>::type a1;
	typename std::decay<A2>::type a2;
	typename std::decay<A3>::type a3;
	typename std::decay<A4>::type a4;
};

////////////////////////////////////////////////////////////////////////////////
/// Member functions callback factories.
////////////////////////////////////////////////////////////////////////////////
template <class Class, class Ret>
CallbackBase *MakeCallback(Class *object, Ret (Class::*func)())
{
	return new CallbackObject0<Class, Ret (Class::*)()>(object, func);
}

template <class Class, class Ret>
CallbackBase *MakeCallback(Class *object, Ret (Class::*func)() const)
{
	return new CallbackObject0<Class, Ret (Class::*)() const>(object, func);
}

template <class Class, class Ret, class A0>
CallbackBase1<A0> *MakeCallback(Class *object, Ret (Class::*func)(A0))
{
	return new CallbackObject1<CallbackBase1<A0>, Class, Ret (Class::*)(A0), A0>(object, func);
}

template <class Class, class Ret, class A0>
CallbackBase1<A0> *MakeCallback(Class *object, Ret (Class::*func)(A0) const)
{
	return new CallbackObject1<CallbackBase1<A0>, Class, Ret (Class::*)(A0) const, A0>(object, func);
}

template <class Class, class Ret, class A0>
CallbackBase*MakeCallback(Class *object, Ret (Class::*func)(A0), A0 a0)
{
	return new CallbackObject1<CallbackBase, Class, Ret (Class::*)(A0), A0>(object, func, a0);
}

template <class Class, class Ret, class A0>
CallbackBase*MakeCallback(Class *object, Ret (Class::*func)(A0) const, A0 a0)
{
	return new CallbackObject1<CallbackBase, Class, Ret (Class::*)(A0) const, A0>(object, func, a0);
}

template <class Class, class Ret, class A0, class A1>
CallbackBase2<A0, A1> *MakeCallback(Class *object, Ret (Class::*func)(A0, A1))
{
	return new CallbackObject2<CallbackBase2<A0, A1>, Class, Ret (Class::*)(A0, A1), A0, A1>(object, func);
}

template <class Class, class Ret, class A0, class A1>
CallbackBase2<A0, A1> *MakeCallback(Class *object, Ret (Class::*func)(A0, A1) const)
{
	return new CallbackObject2<CallbackBase2<A0, A1>, Class, Ret (Class::*)(A0, A1) const, A0, A1>(object, func);
}

template <class Class, class Ret, class A0, class A1>
CallbackBase1<A0> *MakeCallback(Class *object, Ret (Class::*func)(A0, A1), A1 a1)
{
	return new CallbackObject2<CallbackBase1<A0>, Class, Ret (Class::*)(A0, A1), A0, A1>(object, func, a1);
}

template <class Class, class Ret, class A0, class A1>
CallbackBase1<A0> *MakeCallback(Class *object, Ret (Class::*func)(A0, A1) const, A1 a1)
{
	return new CallbackObject2<CallbackBase1<A0>, Class, Ret (Class::*)(A0, A1) const, A0, A1>(object, func, a1);
}

template <class Class, class Ret, class A0, class A1>
CallbackBase *MakeCallback(Class *object, Ret (Class::*func)(A0, A1), A0 a0, A1 a1)
{
	return new CallbackObject2<CallbackBase, Class, Ret (Class::*)(A0, A1), A0, A1>(object, func, a0, a1);
}

template <class Class, class Ret, class A0, class A1>
CallbackBase *MakeCallback(Class *object, Ret (Class::*func)(A0, A1) const, A0 a0, A1 a1)
{
	return new CallbackObject2<CallbackBase, Class, Ret (Class::*)(A0, A1) const, A0, A1>(object, func, a0, a1);
}

template <class Class, class Ret, class A0, class A1, class A2>
CallbackBase3<A0, A1, A2> *MakeCallback(Class *object, Ret (Class::*func)(A0, A1, A2))
{
	return new CallbackObject3<CallbackBase3<A0, A1, A2>, Class, Ret (Class::*)(A0, A1, A2), A0, A1, A2>(object, func);
}

template <class Class, class Ret, class A0, class A1, class A2>
CallbackBase3<A0, A1, A2> *MakeCallback(Class *object, Ret (Class::*func)(A0, A1, A2) const)
{
	return new CallbackObject3<CallbackBase3<A0, A1, A2>, Class, Ret (Class::*)(A0, A1, A2) const, A0, A1, A2>(object, func);
}

template <class Class, class Ret, class A0, class A1, class A2>
CallbackBase2<A0, A1> *MakeCallback(Class *object, Ret (Class::*func)(A0, A1, A2), A2 a2)
{
	return new CallbackObject3<CallbackBase2<A0, A1>, Class, Ret (Class::*)(A0, A1, A2), A0, A1, A2>(object, func, a2);
}

template <class Class, class Ret, class A0, class A1, class A2>
CallbackBase2<A0, A1> *MakeCallback(Class *object, Ret (Class::*func)(A0, A1, A2) const, A2 a2)
{
	return new CallbackObject3<CallbackBase2<A0, A1>, Class, Ret (Class::*)(A0, A1, A2) const, A0, A1, A2>(object, func, a2);
}

template <class Class, class Ret, class A0, class A1, class A2>
CallbackBase1<A0> *MakeCallback(Class *object, Ret (Class::*func)(A0, A1, A2), A1 a1, A2 a2)
{
	return new CallbackObject3<CallbackBase1<A0>, Class, Ret (Class::*)(A0, A1, A2), A0, A1, A2>(object, func, a1, a2);
}

template <class Class, class Ret, class A0, class A1, class A2>
CallbackBase1<A0> *MakeCallback(Class *object, Ret (Class::*func)(A0, A1, A2) const, A1 a1, A2 a2)
{
	return new CallbackObject3<CallbackBase1<A0>, Class, Ret (Class::*)(A0, A1, A2) const, A0, A1, A2>(object, func, a1, a2);
}

template <class Class, class Ret, class A0, class A1, class A2>
CallbackBase *MakeCallback(Class *object, Ret (Class::*func)(A0, A1, A2), A0 a0, A1 a1, A2 a2)
{
	return new CallbackObject3<CallbackBase, Class, Ret (Class::*)(A0, A1, A2), A0, A1, A2>(object, func, a0, a1, a2);
}

template <class Class, class Ret, class A0, class A1, class A2>
CallbackBase *MakeCallback(Class *object, Ret (Class::*func)(A0, A1, A2) const, A0 a0, A1 a1, A2 a2)
{
	return new CallbackObject3<CallbackBase, Class, Ret (Class::*)(A0, A1, A2) const, A0, A1, A2>(object, func, a0, a1, a2);
}

template <class Class, class Ret, class A0, class A1, class A2, class A3>
CallbackBase4<A0, A1, A2, A3> *MakeCallback(Class *object, Ret (Class::*func)(A0, A1, A2, A3))
{
	return new CallbackObject4<CallbackBase4<A0, A1, A2, A3>, Class, Ret (Class::*)(A0, A1, A2, A3), A0, A1, A2, A3>(object, func);
}

template <class Class, class Ret, class A0, class A1, class A2, class A3>
CallbackBase4<A0, A1, A2, A3> *MakeCallback(Class *object, Ret (Class::*func)(A0, A1, A2, A3) const)
{
	return new CallbackObject4<CallbackBase4<A0, A1, A2, A3>, Class, Ret (Class::*)(A0, A1, A2, A3) const, A0, A1, A2, A3>(object, func);
}

template <class Class, class Ret, class A0, class A1, class A2, class A3>
CallbackBase3<A0, A1, A2> *MakeCallback(Class *object, Ret (Class::*func)(A0, A1, A2, A3), A3 a3)
{
	return new CallbackObject4<CallbackBase3<A0, A1, A2>, Class, Ret (Class::*)(A0, A1, A2, A3), A0, A1, A2, A3>(object, func, a3);
}

template <class Class, class Ret, class A0, class A1, class A2, class A3>
CallbackBase3<A0, A1, A2> *MakeCallback(Class *object, Ret (Class::*func)(A0, A1, A2, A3) const, A3 a3)
{
	return new CallbackObject4<CallbackBase3<A0, A1, A2>, Class, Ret (Class::*)(A0, A1, A2, A3) const, A0, A1, A2, A3>(object, func, a3);
}

template <class Class, class Ret, class A0, class A1, class A2, class A3>
CallbackBase2<A0, A1> *MakeCallback(Class *object, Ret (Class::*func)(A0, A1, A2, A3), A2 a2, A3 a3)
{
	return new CallbackObject4<CallbackBase2<A0, A1>, Class, Ret (Class::*)(A0, A1, A2, A3), A0, A1, A2, A3>(object, func, a2, a3);
}

template <class Class, class Ret, class A0, class A1, class A2, class A3>
CallbackBase2<A0, A1> *MakeCallback(Class *object, Ret (Class::*func)(A0, A1, A2, A3) const, A2 a2, A3 a3)
{
	return new CallbackObject4<CallbackBase2<A0, A1>, Class, Ret (Class::*)(A0, A1, A2, A3) const, A0, A1, A2, A3>(object, func, a2, a3);
}

template <class Class, class Ret, class A0, class A1, class A2, class A3>
CallbackBase1<A0> *MakeCallback(Class *object, Ret (Class::*func)(A0, A1, A2, A3), A1 a1, A2 a2, A3 a3)
{
	return new CallbackObject4<CallbackBase1<A0>, Class, Ret (Class::*)(A0, A1, A2, A3), A0, A1, A2, A3>(object, func, a1, a2, a3);
}

template <class Class, class Ret, class A0, class A1, class A2, class A3>
CallbackBase1<A0> *MakeCallback(Class *object, Ret (Class::*func)(A0, A1, A2, A3) const, A1 a1, A2 a2, A3 a3)
{
	return new CallbackObject4<CallbackBase1<A0>, Class, Ret (Class::*)(A0, A1, A2, A3) const, A0, A1, A2, A3>(object, func, a1, a2, a3);
}

template <class Class, class Ret, class A0, class A1, class A2, class A3>
CallbackBase *MakeCallback(Class *object, Ret (Class::*func)(A0, A1, A2, A3), A0 a0, A1 a1, A2 a2, A3 a3)
{
	return new CallbackObject4<CallbackBase, Class, Ret (Class::*)(A0, A1, A2, A3), A0, A1, A2, A3>(object, func, a0, a1, a2, a3);
}

template <class Class, class Ret, class A0, class A1, class A2, class A3>
CallbackBase *MakeCallback(Class *object, Ret (Class::*func)(A0, A1, A2, A3) const, A0 a0, A1 a1, A2 a2, A3 a3)
{
	return new CallbackObject4<CallbackBase, Class, Ret (Class::*)(A0, A1, A2, A3) const, A0, A1, A2, A3>(object, func, a0, a1, a2, a3);
}

template <class Class, class Ret, class A0, class A1, class A2, class A3, class A4>
CallbackBase5<A0, A1, A2, A3, A4> *MakeCallback(Class *object, Ret (Class::*func)(A0, A1, A2, A3, A4))
{
	return new CallbackObject5<CallbackBase5<A0, A1, A2, A3, A4>, Class, Ret (Class::*)(A0, A1, A2, A3, A4), A0, A1, A2, A3, A4>(object, func);
}

template <class Class, class Ret, class A0, class A1, class A2, class A3, class A4>
CallbackBase5<A0, A1, A2, A3, A4> *MakeCallback(Class *object, Ret (Class::*func)(A0, A1, A2, A3, A4) const)
{
	return new CallbackObject5<CallbackBase5<A0, A1, A2, A3, A4>, Class, Ret (Class::*)(A0, A1, A2, A3, A4) const, A0, A1, A2, A3, A4>(object, func);
}

template <class Class, class Ret, class A0, class A1, class A2, class A3, class A4>
CallbackBase4<A0, A1, A2, A3> *MakeCallback(Class *object, Ret (Class::*func)(A0, A1, A2, A3, A4), A4 a4)
{
	return new CallbackObject5<CallbackBase4<A0, A1, A2, A3>, Class, Ret (Class::*)(A0, A1, A2, A3, A4), A0, A1, A2, A3, A4>(object, func, a4);
}

template <class Class, class Ret, class A0, class A1, class A2, class A3, class A4>
CallbackBase4<A0, A1, A2, A3> *MakeCallback(Class *object, Ret (Class::*func)(A0, A1, A2, A3, A4) const, A4 a4)
{
	return new CallbackObject5<CallbackBase4<A0, A1, A2, A3>, Class, Ret (Class::*)(A0, A1, A2, A3, A4) const, A0, A1, A2, A3, A4>(object, func, a4);
}

template <class Class, class Ret, class A0, class A1, class A2, class A3, class A4>
CallbackBase3<A0, A1, A2> *MakeCallback(Class *object, Ret (Class::*func)(A0, A1, A2, A3, A4), A3 a3, A4 a4)
{
	return new CallbackObject5<CallbackBase3<A0, A1, A2>, Class, Ret (Class::*)(A0, A1, A2, A3, A4), A0, A1, A2, A3, A4>(object, func, a3, a4);
}

template <class Class, class Ret, class A0, class A1, class A2, class A3, class A4>
CallbackBase3<A0, A1, A2> *MakeCallback(Class *object, Ret (Class::*func)(A0, A1, A2, A3, A4) const, A3 a3, A4 a4)
{
	return new CallbackObject5<CallbackBase3<A0, A1, A2>, Class, Ret (Class::*)(A0, A1, A2, A3, A4) const, A0, A1, A2, A3, A4>(object, func, a3, a4);
}

template <class Class, class Ret, class A0, class A1, class A2, class A3, class A4>
CallbackBase2<A0, A1> *MakeCallback(Class *object, Ret (Class::*func)(A0, A1, A2, A3, A4), A2 a2, A3 a3, A4 a4)
{
	return new CallbackObject5<CallbackBase2<A0, A1>, Class, Ret (Class::*)(A0, A1, A2, A3, A4), A0, A1, A2, A3, A4>(object, func, a2, a3, a4);
}

template <class Class, class Ret, class A0, class A1, class A2, class A3, class A4>
CallbackBase2<A0, A1> *MakeCallback(Class *object, Ret (Class::*func)(A0, A1, A2, A3, A4) const, A2 a2, A3 a3, A4 a4)
{
	return new CallbackObject5<CallbackBase2<A0, A1>, Class, Ret (Class::*)(A0, A1, A2, A3, A4) const, A0, A1, A2, A3, A4>(object, func, a2, a3, a4);
}

template <class Class, class Ret, class A0, class A1, class A2, class A3, class A4>
CallbackBase1<A0> *MakeCallback(Class *object, Ret (Class::*func)(A0, A1, A2, A3, A4), A1 a1, A2 a2, A3 a3, A4 a4)
{
	return new CallbackObject5<CallbackBase1<A0>, Class, Ret (Class::*)(A0, A1, A2, A3, A4), A0, A1, A2, A3, A4>(object, func, a1, a2, a3, a4);
}

template <class Class, class Ret, class A0, class A1, class A2, class A3, class A4>
CallbackBase1<A0> *MakeCallback(Class *object, Ret (Class::*func)(A0, A1, A2, A3, A4) const, A1 a1, A2 a2, A3 a3, A4 a4)
{
	return new CallbackObject5<CallbackBase1<A0>, Class, Ret (Class::*)(A0, A1, A2, A3, A4) const, A0, A1, A2, A3, A4>(object, func, a1, a2, a3, a4);
}

template <class Class, class Ret, class A0, class A1, class A2, class A3, class A4>
CallbackBase *MakeCallback(Class *object, Ret (Class::*func)(A0, A1, A2, A3, A4), A0 a0, A1 a1, A2 a2, A3 a3, A4 a4)
{
	return new CallbackObject5<CallbackBase, Class, Ret (Class::*)(A0, A1, A2, A3, A4), A0, A1, A2, A3, A4>(object, func, a0, a1, a2, a3, a4);
}

template <class Class, class Ret, class A0, class A1, class A2, class A3, class A4>
CallbackBase *MakeCallback(Class *object, Ret (Class::*func)(A0, A1, A2, A3, A4) const, A0 a0, A1 a1, A2 a2, A3 a3, A4 a4)
{
	return new CallbackObject5<CallbackBase, Class, Ret (Class::*)(A0, A1, A2, A3, A4) const, A0, A1, A2, A3, A4>(object, func, a0, a1, a2, a3, a4);
}

////////////////////////////////////////////////////////////////////////////////
/// Callable callbacks.
////////////////////////////////////////////////////////////////////////////////
template <class T>
class CallbackCallable : public CallbackBase
{
public:
	CallbackCallable(const T& f) : func(f) {}

	virtual void run() { func(); }

private:
	T func;
};

template <class T, class A0>
class CallbackCallable1 : public CallbackBase1<A0>
{
public:
	CallbackCallable1(const T& f) : func(f) {}

	virtual void run() { func(A0{}); }
	virtual void run(A0 arg0) { func(arg0); }

private:
	T func;
};

template <class T, class A0, class A1>
class CallbackCallable2 : public CallbackBase2<A0, A1>
{
public:
	CallbackCallable2(const T& f) : func(f) {}

	virtual void run() { func(A0{}, A1{}); }
	virtual void run(A0 arg0) { func(arg0, A1{}); }
	virtual void run(A0 arg0, A1 arg1) { func(arg0, arg1); }

private:
	T func;
};

template <class T, class A0, class A1, class A2>
class CallbackCallable3 : public CallbackBase3<A0, A1, A2>
{
public:
	CallbackCallable3(const T& f) : func(f) {}

	virtual void run() { func(A0{}, A1{}, A2{}); }
	virtual void run(A0 arg0) { func(arg0, A1{}, A2{}); }
	virtual void run(A0 arg0, A1 arg1, A2 arg2) { func(arg0, arg1, arg2); }

private:
	T func;
};

template <class T, class A0, class A1, class A2, class A3>
class CallbackCallable4 : public CallbackBase4<A0, A1, A2, A3>
{
public:
	CallbackCallable4(const T& f) : func(f) {}

	virtual void run() { func(A0{}, A1{}, A2{}, A3{}); }
	virtual void run(A0 arg0) { func(arg0, A1{}, A2{}, A3{}); }
	virtual void run(A0 arg0, A1 arg1) { func(arg0, arg1, A2{}, A3{}); }
	virtual void run(A0 arg0, A1 arg1, A2 arg2) { func(arg0, arg1, arg2, A3{}); }
	virtual void run(A0 arg0, A1 arg1, A2 arg2, A3 arg3) { func(arg0, arg1, arg2, arg3); }

private:
	T func;
};

template <class T, class A0, class A1, class A2, class A3, class A4>
class CallbackCallable5 : public CallbackBase5<A0, A1, A2, A3, A4>
{
public:
	CallbackCallable5(const T& f) : func(f) {}

	virtual void run() { func(A0{}, A1{}, A2{}, A3{}, A4{}); }
	virtual void run(A0 arg0) { func(arg0, A1{}, A2{}, A3{}, A4{}); }
	virtual void run(A0 arg0, A1 arg1) { func(arg0, arg1, A2{}, A3{}, A4{}); }
	virtual void run(A0 arg0, A1 arg1, A2 arg2) { func(arg0, arg1, arg2, A3{}, A4{}); }
	virtual void run(A0 arg0, A1 arg1, A2 arg2, A3 arg3) { func(arg0, arg1, arg2, arg3, A4{}); }
	virtual void run(A0 arg0, A1 arg1, A2 arg2, A3 arg3, A4 arg4) { func(arg0, arg1, arg2, arg3, arg4); }

private:
	T func;
};

////////////////////////////////////////////////////////////////////////////////
/// Callable callback factories.
////////////////////////////////////////////////////////////////////////////////
template<class T>
class CallableToCallbackTransformer
{
public:
	CallableToCallbackTransformer(const T& f) : func(f) {}

	operator CallbackBase* () { return new CallbackCallable<T>(func); }

	template<class A0>
	operator CallbackBase1<A0>* () {return new CallbackCallable1<T, A0>(func); }

	template<class A0, class A1>
	operator CallbackBase2<A0, A1>* () { return new CallbackCallable2<T, A0, A1>(func); }

	template<class A0, class A1, class A2>
	operator CallbackBase3<A0, A1, A2>* () { return new CallbackCallable3<T, A0, A1, A2>(func); }

	template<class A0, class A1, class A2, class A3>
	operator CallbackBase4<A0, A1, A2, A3>* () { return new CallbackCallable4<T, A0, A1, A2, A3>(func); }

	template<class A0, class A1, class A2, class A3, class A4>
	operator CallbackBase5<A0, A1, A2, A3, A4>* () { return new CallbackCallable5<T, A0, A1, A2, A3, A4>(func); }

private:
	T func;
};

template<class Callable>
CallableToCallbackTransformer<Callable> MakeCallback(const Callable& f) { return f; }

} // namespace Unigine
