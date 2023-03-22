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

#include <UnigineFactory.h>
#include <UnigineInterpreter.h>

namespace Unigine
{

template class UNIGINE_EXPORT Map<String, Variable>;

class UNIGINE_API Exporter
{
public:
	Exporter();
	virtual ~Exporter();

	UNIGINE_INLINE int containsParameter(const char *name) const { return parameters.contains(name); }

	void setParameterInt(const char *name, int v);
	int getParameterInt(const char *name) const;

	void setParameterFloat(const char *name, float v);
	float getParameterFloat(const char *name) const;

	void setParameterDouble(const char *name, double v);
	double getParameterDouble(const char *name) const;

	void setParameterString(const char *name, const char *v);
	const char *getParameterString(const char *name) const;

	int init();
	// 'export' is a keyword in C++.
	int doExport(const NodePtr &root_node, const char *output_filepath);

	void deinit();

protected:
	virtual bool onInit() = 0;
	virtual bool onExport(const NodePtr &root_node, const char *output_filepath) = 0;
	virtual void onDeinit() = 0;

private:
	void set_parameter(const char *name, const Variable &v);
	const Variable &get_parameter(const char *name, const Variable &def) const;

private:
	Map<String, Variable> parameters;

};

template class UNIGINE_EXPORT Map<String, Factory<String, Exporter>::FuncPtr>;
template class UNIGINE_EXPORT Factory<String, Exporter>;
template class UNIGINE_EXPORT Map<String, String>;
template class UNIGINE_EXPORT Map<String, Vector<String>>;

class UNIGINE_API Export
{
	Export();
	~Export();
public:

	static Export * get();

	template<typename Type>
	void registerExporter(const char *type_name, const Vector<String> &extensions)
	{
		extensions_by_type.append(type_name, extensions);
		for (const String &ext : extensions)
		{
			if (type_by_extension.contains(ext))
			{
				Log::warning("Extensions \"%s\" already registered.\n", ext.get());
				continue;
			}
			type_by_extension.append(ext, type_name);
		}
		exporters_factory.append<Type>(type_name);
	}

	bool unregisterExporter(const char *type_name);

	Exporter *createExporter(const char *type_name) const;
	Exporter *createExporterByFileName(const char *file_name) const;

	Vector<String> getExporterTypes() const;
	void getExporterTypes(Vector<String> &types) const;

	bool isSupportedExtension(const char *extension) const;
	Vector<String> getSupportedExtensions() const;
	void getSupportedExtensions(Vector<String> &extensions) const;

	String getExporterTypeByExtension(const char *extension) const;

	bool doExport(const NodePtr& root_node, const char *output_filepath) const;

private:
	void export_info();

private:
	Factory<String, Exporter> exporters_factory;
	Map<String, String> type_by_extension;
	Map<String, Vector<String>> extensions_by_type;

};

} //namespace Unigine
