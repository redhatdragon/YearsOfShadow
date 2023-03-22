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

#include <editor/UnigineEditorGlobal.h>

#include <UnigineString.h>
#include <UnigineGUID.h>
#include <UnigineMathLib.h>

#include <memory>	// TODO(victor: 6/6/22): remove after rewriting as Unigine based API


namespace UnigineEditor
{

class UNIGINE_EDITOR_API SettingsParameters
{
public:
	virtual ~SettingsParameters();

	virtual bool contains(const char *full_parameter_name) const = 0;
	virtual bool remove(const char *full_parameter_name) = 0;

	virtual void setInt(const char *full_parameter_name, int value) = 0;
	virtual int getInt(const char *full_parameter_name, int default_value = 0) const = 0;

	virtual void setInt64(const char *full_parameter_name, int64_t value) = 0;
	virtual int64_t getInt64(const char *full_parameter_name, int64_t default_value = 0) const = 0;

	virtual void setFloat(const char *full_parameter_name, float value) = 0;
	virtual float getFloat(const char *full_parameter_name, float default_value = 0) const = 0;

	virtual void setDouble(const char *full_parameter_name, double value) = 0;
	virtual double getDouble(const char *full_parameter_name, double default_value = 0) const = 0;

	virtual void setBool(const char *full_parameter_name, bool value) = 0;
	virtual bool getBool(const char *full_parameter_name, bool default_value = false) const = 0;

	virtual void setGUID(const char *full_parameter_name, const Unigine::UGUID &value) = 0;
	virtual Unigine::UGUID getGUID(const char *full_parameter_name,
		const Unigine::UGUID &default_value = Unigine::UGUID::empty) const = 0;

	virtual void setString(const char *full_parameter_name, const char *value) = 0;
	virtual Unigine::String getString(const char *full_parameter_name,
		const char *default_value = nullptr) const = 0;

	virtual void setIVec2(const char *full_parameter_name, const Unigine::Math::ivec2 &value) = 0;
	virtual Unigine::Math::ivec2 getIVec2(const char *full_parameter_name,
		const Unigine::Math::ivec2 &default_value = Unigine::Math::ivec2_zero) const = 0;

	virtual void setIVec3(const char *full_parameter_name, const Unigine::Math::ivec3 &value) = 0;
	virtual Unigine::Math::ivec3 getIVec3(const char *full_parameter_name,
		const Unigine::Math::ivec3 &default_value = Unigine::Math::ivec3_zero) const = 0;

	virtual void setIVec4(const char *full_parameter_name, const Unigine::Math::ivec4 &value) = 0;
	virtual Unigine::Math::ivec4 getIVec4(const char *full_parameter_name,
		const Unigine::Math::ivec4 &default_value = Unigine::Math::ivec4_zero) const = 0;

	virtual void setVec2(const char *full_parameter_name, const Unigine::Math::vec2 &value) = 0;
	virtual Unigine::Math::vec2 getVec2(const char *full_parameter_name,
		const Unigine::Math::vec2 &default_value = Unigine::Math::vec2_zero) const = 0;

	virtual void setVec3(const char *full_parameter_name, const Unigine::Math::vec3 &value) = 0;
	virtual Unigine::Math::vec3 getVec3(const char *full_parameter_name,
		const Unigine::Math::vec3 &default_value = Unigine::Math::vec3_zero) const = 0;

	virtual void setVec4(const char *full_parameter_name, const Unigine::Math::vec4 &value) = 0;
	virtual Unigine::Math::vec4 getVec4(const char *full_parameter_name,
		const Unigine::Math::vec4 &default_value = Unigine::Math::vec4_zero) const = 0;

	virtual void setDVec2(const char *full_parameter_name, const Unigine::Math::dvec2 &value) = 0;
	virtual Unigine::Math::dvec2 getDVec2(const char *full_parameter_name,
		const Unigine::Math::dvec2 &default_value = Unigine::Math::dvec2_zero) const = 0;

	virtual void setDVec3(const char *full_parameter_name, const Unigine::Math::dvec3 &value) = 0;
	virtual Unigine::Math::dvec3 getDVec3(const char *full_parameter_name,
		const Unigine::Math::dvec3 &default_value = Unigine::Math::dvec3_zero) const = 0;

	virtual void setDVec4(const char *full_parameter_name, const Unigine::Math::dvec4 &value) = 0;
	virtual Unigine::Math::dvec4 getDVec4(const char *full_parameter_name,
		const Unigine::Math::dvec4 &default_value = Unigine::Math::dvec4_zero) const = 0;
};

class UNIGINE_EDITOR_API UserSettings : public SettingsParameters
{
public:
	virtual const char *getPath() const = 0;
};

class UNIGINE_EDITOR_API SettingManager final
{
public:
	static bool isInitialized();
	static UserSettings *userSettings();
	// TODO(victor: 6/6/22): In the future, when we rewrite this file like Unigine based API,
	// shared_ptr will be replaced with Ptr.
	// TODO(victor: 6/6/22): make a clear name for this function, because you create settings which
	// will be saved for the project by some specific module.
	// May be it's necessary to think of names for all these functions.
	static std::shared_ptr<SettingsParameters> projectSettings(const char *config_name);
};

namespace Utils
{

class SettingsParametersGroup
{
public:
	SettingsParametersGroup() = delete;
	explicit SettingsParametersGroup(SettingsParameters *s, const char *group_name)
		: sp_{s}
		, prefix_(group_name)
	{}

	bool contains(const char *parameter_name) const
	{
		return sp_->contains(makeFullName(parameter_name).get());
	}
	void remove(const char *parameter_name)
	{
		sp_->remove(makeFullName(parameter_name).get());
	}

	void setInt(const char *parameter_name, int value)
	{
		sp_->setInt(makeFullName(parameter_name).get(), value);
	}
	int getInt(const char *parameter_name, int default_value = 0) const
	{
		return sp_->getInt(makeFullName(parameter_name).get(), default_value);
	}

	void setInt64(const char *parameter_name, int64_t value)
	{
		sp_->setInt64(makeFullName(parameter_name).get(), value);
	}
	int64_t getInt64(const char *parameter_name, int64_t default_value = 0) const
	{
		return sp_->getInt64(makeFullName(parameter_name).get(), default_value);
	}

	void setFloat(const char *parameter_name, float value)
	{
		sp_->setFloat(makeFullName(parameter_name).get(), value);
	}
	float getFloat(const char *parameter_name, float default_value = 0.0) const
	{
		return sp_->getFloat(makeFullName(parameter_name).get(), default_value);
	}

	void setDouble(const char *parameter_name, double value)
	{
		sp_->setDouble(makeFullName(parameter_name).get(), value);
	}
	double getDouble(const char *parameter_name, double default_value = 0.0) const
	{
		return sp_->getDouble(makeFullName(parameter_name).get(), default_value);
	}

	void setBool(const char *parameter_name, bool value)
	{
		sp_->setBool(makeFullName(parameter_name).get(), value);
	}
	bool getBool(const char *parameter_name, bool default_value = 0) const
	{
		return sp_->getBool(makeFullName(parameter_name).get(), default_value);
	}

	void setGUID(const char *parameter_name, const Unigine::UGUID &value)
	{
		sp_->setGUID(makeFullName(parameter_name).get(), value);
	}
	Unigine::UGUID getGUID(const char *parameter_name,
		const Unigine::UGUID &default_value = Unigine::UGUID::empty) const
	{
		return sp_->getGUID(makeFullName(parameter_name).get(), default_value);
	}

	void setString(const char *parameter_name, const char *value)
	{
		sp_->setString(makeFullName(parameter_name).get(), value);
	}
	Unigine::String getString(const char *parameter_name,
		const char *default_value = nullptr) const
	{
		return sp_->getString(makeFullName(parameter_name).get(), default_value);
	}

	void setIVec2(const char *parameter_name, const Unigine::Math::ivec2 &value)
	{
		sp_->setIVec2(makeFullName(parameter_name).get(), value);
	}
	Unigine::Math::ivec2 getIVec2(const char *parameter_name,
		const Unigine::Math::ivec2 &default_value = Unigine::Math::ivec2_zero) const
	{
		return sp_->getIVec2(makeFullName(parameter_name).get(), default_value);
	}

	void setIVec3(const char *parameter_name, const Unigine::Math::ivec3 &value)
	{
		sp_->setIVec3(makeFullName(parameter_name).get(), value);
	}
	Unigine::Math::ivec3 getIVec3(const char *parameter_name,
		const Unigine::Math::ivec3 &default_value = Unigine::Math::ivec3_zero) const
	{
		return sp_->getIVec3(makeFullName(parameter_name).get(), default_value);
	}

	void setIVec4(const char *parameter_name, const Unigine::Math::ivec4 &value)
	{
		sp_->setIVec4(makeFullName(parameter_name).get(), value);
	}
	Unigine::Math::ivec4 getIVec4(const char *parameter_name,
		const Unigine::Math::ivec4 &default_value = Unigine::Math::ivec4_zero) const
	{
		return sp_->getIVec4(makeFullName(parameter_name).get(), default_value);
	}

	void setVec2(const char *parameter_name, const Unigine::Math::vec2 &value)
	{
		sp_->setVec2(makeFullName(parameter_name).get(), value);
	}
	Unigine::Math::vec2 getVec2(const char *parameter_name,
		const Unigine::Math::vec2 &default_value = Unigine::Math::vec2_zero) const
	{
		return sp_->getVec2(makeFullName(parameter_name).get(), default_value);
	}

	void setVec3(const char *parameter_name, const Unigine::Math::vec3 &value)
	{
		sp_->setVec3(makeFullName(parameter_name).get(), value);
	}
	Unigine::Math::vec3 getVec3(const char *parameter_name,
		const Unigine::Math::vec3 &default_value = Unigine::Math::vec3_zero) const
	{
		return sp_->getVec3(makeFullName(parameter_name).get(), default_value);
	}

	void setVec4(const char *parameter_name, const Unigine::Math::vec4 &value)
	{
		sp_->setVec4(makeFullName(parameter_name).get(), value);
	}
	Unigine::Math::vec4 getVec4(const char *parameter_name,
		const Unigine::Math::vec4 &default_value = Unigine::Math::vec4_zero) const
	{
		return sp_->getVec4(makeFullName(parameter_name).get(), default_value);
	}

	void setDVec2(const char *parameter_name, const Unigine::Math::dvec2 &value)
	{
		sp_->setDVec2(makeFullName(parameter_name).get(), value);
	}
	Unigine::Math::dvec2 getDVec2(const char *parameter_name,
		const Unigine::Math::dvec2 &default_value = Unigine::Math::dvec2_zero) const
	{
		return sp_->getDVec2(makeFullName(parameter_name).get(), default_value);
	}

	void setDVec3(const char *parameter_name, const Unigine::Math::dvec3 &value)
	{
		sp_->setDVec3(makeFullName(parameter_name).get(), value);
	}
	Unigine::Math::dvec3 getDVec3(const char *parameter_name,
		const Unigine::Math::dvec3 &default_value = Unigine::Math::dvec3_zero) const
	{
		return sp_->getDVec3(makeFullName(parameter_name).get(), default_value);
	}

	void setDVec4(const char *parameter_name, const Unigine::Math::dvec4 &value)
	{
		sp_->setDVec4(makeFullName(parameter_name).get(), value);
	}

	Unigine::Math::dvec4 getDVec4(const char *parameter_name,
		const Unigine::Math::dvec4 &default_value = Unigine::Math::dvec4_zero) const
	{
		return sp_->getDVec4(makeFullName(parameter_name).get(), default_value);
	}

	Unigine::StringStack<> makeFullName(const char *parameter_name) const
	{
		return prefix_ + parameter_name;
	}

private:
	SettingsParameters *sp_{};
	Unigine::StringStack<> prefix_;
};

} // namespace Utils
} // namespace UnigineEditor
