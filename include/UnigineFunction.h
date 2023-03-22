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

#include "UnigineEngine.h"
#include "UnigineString.h"
#include "UnigineInterpreter.h"

/**
 * Unigine namespace.
 */
namespace Unigine
{

/**
 * Function base class.
 */
class FunctionBase
{

public:

	/**
	 * Default constructor.
	 */
	FunctionBase(const Variable &name): name(name) {}
	/**
	 * Virtual destructor.
	 */
	virtual ~FunctionBase() {}

	/**
	 * Sets the function name.
	 *
	 * @param n Function name.
	 */
	void setName(const Variable &n) { name = n; }
	/**
	 * Returns the function name.
	 *
	 * @return Function name.
	 */
	const Variable &getName() const { return name.get(); }

	/**
	 * Runs the system script function.
	 *
	 * @return System script function return value.
	 */
	Variable runSystem() const { return Engine::get()->runSystemFunction(name.get(), get_args(), getNumArgs()); }
	/**
	 * Runs the world script function.
	 *
	 * @return World script function return value.
	 */
	Variable runWorld() const { return Engine::get()->runWorldFunction(name.get(), get_args(), getNumArgs()); }
	/**
	 * Runs the editor script function.
	 *
	 * @return Editor script function return value.
	 */
	Variable runEditor() const { return Engine::get()->runEditorFunction(name.get(), get_args(), getNumArgs()); }

	/**
	 * Returns the number of arguments.
	 *
	 * @return Number of arguments.
	 */
	virtual int getNumArgs() const = 0;
	/**
	 * Sets an argument value.
	 *
	 * @param num Argument number.
	 * @param a Argument value.
	 */
	void setArg(int num, const Variable &a)
	{
		UNIGINE_ASSERT(num >= 0 && num < getNumArgs() && "Unigine::FunctionBase::setArg(): bad argument");
		get_args()[num] = a;
	}
	/**
	 * Gets the argument value.
	 *
	 * @param num Argument number.
	 * @return Argument value.
	 */
	const Variable &getArg(int num)
	{
		UNIGINE_ASSERT(num >= 0 && num < getNumArgs() && "Unigine::FunctionBase::getArg(): bad argument");
		return get_args()[num];
	}

protected:

	/// @cond

	virtual const Variable *get_args() const = 0;
	virtual Variable *get_args() = 0;

	Variable name;

	/// @endcond
};

/// @cond

/*
 */
class Function0: public FunctionBase
{
public:
	Function0(const Variable &name): FunctionBase(name) {}
	virtual int getNumArgs() const { return 0; }
private:
	virtual const Variable *get_args() const { return NULL; }
	virtual Variable *get_args() { return NULL; }
};

class Function1: public FunctionBase
{
public:
	Function1(const Variable &name): FunctionBase(name) {}
	Function1(const Variable &name, const Variable &a0): FunctionBase(name)
	{
		args[0] = a0;
	}
	virtual int getNumArgs() const { return 1; }
private:
	virtual const Variable *get_args() const { return args; }
	virtual Variable *get_args() { return args; }
	Variable args[1];
};

class Function2: public FunctionBase
{
public:
	Function2(const Variable &name): FunctionBase(name) {}
	Function2(const Variable &name, const Variable &a0, const Variable &a1): FunctionBase(name)
	{
		args[0] = a0; args[1] = a1;
	}
	virtual int getNumArgs() const { return 2; }
private:
	virtual const Variable *get_args() const { return args; }
	virtual Variable *get_args() { return args; }
	Variable args[2];
};

class Function3: public FunctionBase
{
public:
	Function3(const Variable &name): FunctionBase(name) {}
	Function3(const Variable &name, const Variable &a0, const Variable &a1, const Variable &a2): FunctionBase(name)
	{
		args[0] = a0; args[1] = a1; args[2] = a2;
	}
	virtual int getNumArgs() const { return 3; }
private:
	virtual const Variable *get_args() const { return args; }
	virtual Variable *get_args() { return args; }
	Variable args[3];
};

class Function4: public FunctionBase
{
public:
	Function4(const Variable &name): FunctionBase(name) {}
	Function4(const Variable &name, const Variable &a0, const Variable &a1, const Variable &a2, const Variable &a3): FunctionBase(name)
	{
		args[0] = a0; args[1] = a1; args[2] = a2; args[3] = a3;
	}
	virtual int getNumArgs() const { return 4; }
private:
	virtual const Variable *get_args() const { return args; }
	virtual Variable *get_args() { return args; }
	Variable args[4];
};

class Function5: public FunctionBase
{
public:
	Function5(const Variable &name): FunctionBase(name) {}
	Function5(const Variable &name, const Variable &a0, const Variable &a1, const Variable &a2, const Variable &a3, const Variable &a4): FunctionBase(name)
	{
		args[0] = a0; args[1] = a1; args[2] = a2; args[3] = a3; args[4] = a4;
	}
	virtual int getNumArgs() const { return 5; }
private:
	virtual const Variable *get_args() const { return args; }
	virtual Variable *get_args() { return args; }
	Variable args[5];
};

class Function6: public FunctionBase
{
public:
	Function6(const Variable &name): FunctionBase(name) {}
	Function6(const Variable &name, const Variable &a0, const Variable &a1, const Variable &a2, const Variable &a3, const Variable &a4, const Variable &a5): FunctionBase(name)
	{
		args[0] = a0; args[1] = a1; args[2] = a2; args[3] = a3; args[4] = a4; args[5] = a5;
	}
	virtual int getNumArgs() const { return 6; }
private:
	virtual const Variable *get_args() const { return args; }
	virtual Variable *get_args() { return args; }
	Variable args[6];
};

class Function7: public FunctionBase
{
public:
	Function7(const Variable &name): FunctionBase(name) {}
	Function7(const Variable &name, const Variable &a0, const Variable &a1, const Variable &a2, const Variable &a3, const Variable &a4, const Variable &a5, const Variable &a6): FunctionBase(name)
	{
		args[0] = a0; args[1] = a1; args[2] = a2; args[3] = a3; args[4] = a4; args[5] = a5; args[6] = a6;
	}
	virtual int getNumArgs() const { return 7; }
private:
	virtual const Variable *get_args() const { return args; }
	virtual Variable *get_args() { return args; }
	Variable args[7];
};

class Function8: public FunctionBase
{
public:
	Function8(const Variable &name): FunctionBase(name) {}
	Function8(const Variable &name, const Variable &a0, const Variable &a1, const Variable &a2, const Variable &a3, const Variable &a4, const Variable &a5, const Variable &a6, const Variable &a7): FunctionBase(name)
	{
		args[0] = a0; args[1] = a1; args[2] = a2; args[3] = a3; args[4] = a4; args[5] = a5; args[6] = a6; args[7] = a7;
	}
	virtual int getNumArgs() const { return 8; }
private:
	virtual const Variable *get_args() const { return args; }
	virtual Variable *get_args() { return args; }
	Variable args[8];
};

class Function9: public FunctionBase
{
public:
	Function9(const Variable &name): FunctionBase(name) {}
	Function9(const Variable &name, const Variable &a0, const Variable &a1, const Variable &a2, const Variable &a3, const Variable &a4, const Variable &a5, const Variable &a6, const Variable &a7, const Variable &a8): FunctionBase(name)
	{
		args[0] = a0; args[1] = a1; args[2] = a2; args[3] = a3; args[4] = a4; args[5] = a5; args[6] = a6; args[7] = a7; args[8] = a8;
	}
	virtual int getNumArgs() const { return 9; }
private:
	virtual const Variable *get_args() const { return args; }
	virtual Variable *get_args() { return args; }
	Variable args[9];
};

/// @endcond

/**
 * Makes a script function. The target function receives up to 9 arguments.
 *
 * @return Pointer to the function.
 */
UNIGINE_INLINE FunctionBase *MakeFunction(const Variable &name)
{
	return new Function0(name);
}

/**
 * Makes a script function. The target function receives up to 9 arguments.
 *
 * @param name Function name.
 * @param num Number of arguments.
 * @return Pointer to the function.
 */
UNIGINE_INLINE FunctionBase *MakeFunction(const Variable &name, int num)
{
	switch (num)
	{
		case 0: return new Function0(name);
		case 1: return new Function1(name);
		case 2: return new Function2(name);
		case 3: return new Function3(name);
		case 4: return new Function4(name);
		case 5: return new Function5(name);
		case 6: return new Function6(name);
		case 7: return new Function7(name);
		case 8: return new Function8(name);
		case 9: return new Function9(name);
	}
	UNIGINE_ASSERT(0 && "Unigine::MakeFunction(): bad number of argument");
	return 0;
}

/// @cond

UNIGINE_INLINE FunctionBase *MakeFunction(const Variable &name, const Variable &a0)
{
	return new Function1(name, a0);
}

UNIGINE_INLINE FunctionBase *MakeFunction(const Variable &name, const Variable &a0, const Variable &a1)
{
	return new Function2(name, a0, a1);
}

UNIGINE_INLINE FunctionBase *MakeFunction(const Variable &name, const Variable &a0, const Variable &a1, const Variable &a2)
{
	return new Function3(name, a0, a1, a2);
}

UNIGINE_INLINE FunctionBase *MakeFunction(const Variable &name, const Variable &a0, const Variable &a1, const Variable &a2, const Variable &a3)
{
	return new Function4(name, a0, a1, a2, a3);
}

UNIGINE_INLINE FunctionBase *MakeFunction(const Variable &name, const Variable &a0, const Variable &a1, const Variable &a2, const Variable &a3, const Variable &a4)
{
	return new Function5(name, a0, a1, a2, a3, a4);
}

UNIGINE_INLINE FunctionBase *MakeFunction(const Variable &name, const Variable &a0, const Variable &a1, const Variable &a2, const Variable &a3, const Variable &a4, const Variable &a5)
{
	return new Function6(name, a0, a1, a2, a3, a4, a5);
}

UNIGINE_INLINE FunctionBase *MakeFunction(const Variable &name, const Variable &a0, const Variable &a1, const Variable &a2, const Variable &a3, const Variable &a4, const Variable &a5, const Variable &a6)
{
	return new Function7(name, a0, a1, a2, a3, a4, a5, a6);
}

UNIGINE_INLINE FunctionBase *MakeFunction(const Variable &name, const Variable &a0, const Variable &a1, const Variable &a2, const Variable &a3, const Variable &a4, const Variable &a5, const Variable &a6, const Variable &a7)
{
	return new Function8(name, a0, a1, a2, a3, a4, a5, a6, a7);
}

UNIGINE_INLINE FunctionBase *MakeFunction(const Variable &name, const Variable &a0, const Variable &a1, const Variable &a2, const Variable &a3, const Variable &a4, const Variable &a5, const Variable &a6, const Variable &a7, const Variable &a8)
{
	return new Function9(name, a0, a1, a2, a3, a4, a5, a6, a7, a8);
}

/// @endcond

} /* namespace Unigine */
