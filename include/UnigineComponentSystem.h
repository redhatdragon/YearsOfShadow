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
#include "UnigineNode.h"
#include "UnigineMap.h"
#include "UnigineLogic.h"
#include "UnigineProperties.h"
#include "UnigineMaterials.h"
#include "UnigineCurve2d.h"
#include "UnigineFileSystem.h"
#include "UnigineCallback.h"
#include "UnigineThread.h"
#include "UnigineDir.h"
#include "UnigineWorld.h"

// Component example (.h file)
/*
#pragma once
#include <UnigineComponentSystem.h>

class MyComponent: public Unigine::ComponentBase
{
public:
	COMPONENT_DEFINE(MyComponent, Unigine::ComponentBase);
	COMPONENT_DESCRIPTION(
		"You can use this component on rigidbody objects only\n"
		"This component is implicitly related to YourComponent"
	);

	// methods
	COMPONENT_INIT(init);
	COMPONENT_UPDATE(update);

	// parameters
	PROP_GROUP("Very Important Parameters");
	PROP_PARAM(Float, my_float, 1.5f);
	PROP_PARAM(Node, my_node);
	PROP_PARAM(Switch, my_type, 0, "one,two,three", "Title", "ToolTip for param", "Group");
	// last parameter "args" - for specify attributes "name1=value1;name2=value2" (https://developer.unigine.com/en/docs/2.14.1/code/formats/property_format?rlang=cpp#element_parameter)
	PROP_PARAM(Float, my_float_2, 0.0f, "Float Title", "Float ToolTip for param", "Group", "my_type=2;max=10;min=-10");

protected:
	// world main loop
	void init();
	void update();
};
*/

// Component example (.cpp file)
/*
#include "MyComponent.h"

REGISTER_COMPONENT(MyComponent);

using namespace Unigine;
using namespace Math;

void MyComponent::init()
{
}

void MyComponent::update()
{
}
*/

#if defined(UNIGINE) && !defined(_WIN32)
	#define UNIGINE_CS	__attribute__((visibility("default")))
#else
	#define UNIGINE_CS
#endif

namespace Unigine
{
class ComponentBase;

////////////////////////////////////////////////////////////////////////////////////////
// Component Macros
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __GNUC__
#pragma region Macros
#endif

// component
#define COMPONENT(CLASS_NAME, PARENT_NAME)																	\
	public:																									\
		CLASS_NAME(const Unigine::NodePtr &node, int num) : PARENT_NAME(node, num) {}						\
		virtual ~CLASS_NAME() {}																			\
		using __this_class = CLASS_NAME;																	\
		virtual bool isBase() const override { return std::is_same<PARENT_NAME, ComponentBase>::value; }	\
		const char *getClassName() const override { return #CLASS_NAME; }									\
		PROP_TITLE(nullptr);																				\
		PROP_TOOLTIP(nullptr);																				\
		PROP_GROUP(nullptr);

#define COMPONENT_DEFINE(CLASS_NAME, PARENT_NAME)	\
COMPONENT(CLASS_NAME, PARENT_NAME);					\
PROP_NAME(#CLASS_NAME);

// component description
#define COMPONENT_DESCRIPTION(TEXT) const char *getComponentDescription() const override { return TEXT; }

// property
#define PROP_NAME(NAME) static const char *getPropertyName() { return NAME; }
#define PROP_PARENT_NAME(PARENT_NAME) const char *getParentPropertyName() const override { return PARENT_NAME; }
#define PROP_AUTOSAVE(VALUE) int isAutoSaveProperty() const override { return VALUE; }

// parameters
#define PROP_PARAM(TYPE, NAME, ...) \
	Unigine::ComponentVariable##TYPE NAME { this, #NAME, ##__VA_ARGS__ };
#define PROP_STRUCT(TYPE, NAME, ...) \
	Unigine::ComponentVariableStruct<TYPE> NAME { this, #TYPE, #NAME, ##__VA_ARGS__ };
#define PROP_ARRAY(TYPE, NAME, ...) \
	Unigine::ComponentVariableArray<Unigine::ComponentVariable##TYPE> NAME { this, #NAME, #TYPE, ##__VA_ARGS__ };
#define PROP_ARRAY_STRUCT(TYPE, NAME, ...) \
	Unigine::ComponentVariableArray<Unigine::ComponentVariableStruct<TYPE>> NAME { this, #NAME, #TYPE, ##__VA_ARGS__ };
// title, tooltip, group
// ( creates something like: Unigine::ComponentVariableArgumentGroup __group_23 { "MyPrettyGroup" }; )
#define PROP_TITLE(TEXT) Unigine::ComponentVariableArgumentTitle UNIGINE_CONCATENATE(__title_, __LINE__) { TEXT };
#define PROP_TOOLTIP(TEXT) Unigine::ComponentVariableArgumentTooltip UNIGINE_CONCATENATE(__tooltip_, __LINE__) { TEXT };
#define PROP_GROUP(TEXT) Unigine::ComponentVariableArgumentGroup UNIGINE_CONCATENATE(__group_, __LINE__) { TEXT };

// one level of macro indirection is required in order to resolve __LINE__,
// and get __group_10 instead of __group__LINE__
#ifndef UNIGINE_CONCATENATE
#define UNIGINE_CONCATENATE_IMPL(A, B) A ## B
#define UNIGINE_CONCATENATE(A, B) UNIGINE_CONCATENATE_IMPL(A, B)
#endif
// register methods
#define COMPONENT_METHOD(TYPE, NAME, ...)																					\
	template <typename T>																									\
	class ComponentMethodRegistrator_##NAME																					\
	{																														\
	public:																													\
		Unigine::ComponentBase *component;																					\
		Unigine::CallbackBase *func;																						\
		Unigine::String name;																								\
		ComponentMethodRegistrator_##NAME(T *c)																				\
		{																													\
			component = c;																									\
			func = Unigine::MakeCallback(c, &T::NAME);																		\
			name = Unigine::String::format("%s::%s", c->getClassName(), #NAME);												\
			Unigine::ComponentSystem::get()->addComponentMethod##TYPE(component, func, name.get(), #NAME, ##__VA_ARGS__);	\
		}																													\
		~ComponentMethodRegistrator_##NAME()																				\
		{																													\
			Unigine::ComponentSystem::get()->removeComponentMethod##TYPE(component, func, name.get(), #NAME, ##__VA_ARGS__);\
			delete func;																									\
		}																													\
	};																														\
	ComponentMethodRegistrator_##NAME<__this_class> __method_registrator_##NAME{ this };

#define COMPONENT_INIT(NAME, ...)						COMPONENT_METHOD(Init, NAME, ##__VA_ARGS__)
#define COMPONENT_UPDATE_ASYNC_THREAD(NAME, ...)		COMPONENT_METHOD(UpdateAsyncThread, NAME, ##__VA_ARGS__)
#define COMPONENT_UPDATE_SYNC_THREAD(NAME, ...)			COMPONENT_METHOD(UpdateSyncThread, NAME, ##__VA_ARGS__)
#define COMPONENT_UPDATE(NAME, ...)						COMPONENT_METHOD(Update, NAME, ##__VA_ARGS__)
#define COMPONENT_POST_UPDATE(NAME, ...)				COMPONENT_METHOD(PostUpdate, NAME, ##__VA_ARGS__)
#define COMPONENT_UPDATE_PHYSICS(NAME, ...)				COMPONENT_METHOD(UpdatePhysics, NAME, ##__VA_ARGS__)
#define COMPONENT_SWAP(NAME, ...)						COMPONENT_METHOD(Swap, NAME, ##__VA_ARGS__)
#define COMPONENT_SHUTDOWN(NAME, ...)					COMPONENT_METHOD(Shutdown, NAME, ##__VA_ARGS__)

// register component in the ComponentSystem
// you can use this macro instead of using ComponentSystem::get()->registerComponent<CLASS_NAME>();
// put it in your .cpp file
#define REGISTER_COMPONENT(CLASS_NAME)																											\
class ComponentRegistrator_##CLASS_NAME																											\
{																																				\
public:																																			\
	ComponentRegistrator_##CLASS_NAME()																											\
	{																																			\
		Unigine::ComponentSystem::get()->addInitCallback(Unigine::MakeCallback(this, &ComponentRegistrator_##CLASS_NAME::register_component));	\
	}																																			\
	void register_component()																													\
	{																																			\
		Unigine::ComponentSystem::get()->registerComponent<CLASS_NAME>();																		\
	}																																			\
	~ComponentRegistrator_##CLASS_NAME()																										\
	{																																			\
		Unigine::ComponentSystem::get()->unregisterComponent<CLASS_NAME>();																		\
	}																																			\
} __component_registrator_##CLASS_NAME;

#ifndef __GNUC__
#pragma endregion Macros
#endif

////////////////////////////////////////////////////////////////////////////////////////
// Component Factory
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __GNUC__
#pragma region Factory
#endif

class ComponentCreatorInterface : public Base
{
public:
	virtual ~ComponentCreatorInterface() {}
	virtual ComponentBase *create(const NodePtr &node, int num) const = 0;
};

template <class C>
class ComponentCreator : public ComponentCreatorInterface
{
public:
	virtual ComponentBase *create(const NodePtr &node, int num) const { return new C(node, num); }
};

class ComponentFactory : public Base
{
public:
	ComponentFactory() {}

	virtual ~ComponentFactory()
	{
		for (auto i = map.begin(); i != map.end(); ++i)
			delete i->data;
	}

	template <class C>
	void add(const char *name)
	{
		String str(name);
		auto creator = map.find(str);
		if (creator == map.end())
			map.append(str, new ComponentCreator<C>());
	}

	template <class C>
	void remove(const char *name)
	{
		String str(name);
		auto creator = map.find(str);
		if (creator != map.end())
		{
			delete creator->data;
			map.remove(creator);
		}
	}

	ComponentBase *create(const char *name, const NodePtr &node, int num) const
	{
		String str(name);
		auto o = map.find(str);
		if (o != map.end())
			return o->data->create(node, num);
		return nullptr;
	}

	void getComponentNames(Vector<String> &names) const
	{
		map.getKeys(names);
	}

private:
	Map<String, ComponentCreatorInterface *> map; // <property name, component creator>
};

#ifndef __GNUC__
#pragma endregion Factory
#endif

////////////////////////////////////////////////////////////////////////////////////////
// Component System
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __GNUC__
#pragma region System
#endif

class ComponentSystem : private WorldLogic
{
public:
	// return pointer to singleton ComponentSystem
	UNIGINE_API static ComponentSystem *get();

	// initialize Component System and run all "init callbacks"
	UNIGINE_API void initialize();

	// adds a callback that is called during initialize()
	UNIGINE_API void addInitCallback(CallbackBase *callback);

	// for perf debug modes : enable/disable update, postUpdate, updatePhysics, swap, updateSync, updateAsync for all components
	void setEnabled(bool enabled) { is_enabled = enabled; }
	bool isEnabled() const { return is_enabled; }

	// register user object derived from ComponentBase class
	template <class C>
	void registerComponent()
	{
		component_factory.add<C>(C::getPropertyName());

		// create property file and load if it isn't exist
		PropertyPtr prop = Properties::findManualProperty(C::getPropertyName());
		if (!prop)
			createPropertyFile(C::getPropertyName());
		else
		{
			// If world was loaded before registerComponent gets called, there might be potential instances
			// of that component, that weren't created when addProperty was called before registration.
			int count = prop->getNumChildren();
			for (int i = 0; i < count; i++)
			{
				PropertyPtr child_prop = prop->getChild(i);
				NodePtr node = child_prop->getNode();
				if (node)
				{
					if (components.find(node->getID()) == components.end() || components[node->getID()].size() != node->getNumProperties())
						on_property_slots_changed(node, node->getNumProperties());

					on_property_created(node, child_prop, node->findProperty(child_prop));
				}
			}
		}
	}

	template <class C>
	void unregisterComponent()
	{
		component_factory.remove<C>(C::getPropertyName());

		// Delete all instances of a component.
		// If the module containing the component is unloaded while the world is still loaded,
		// and if there are instances of those components in the world, they will crash due to dangling pointers.
		for (auto it = components.begin(); it != components.end(); ++it)
		{
			int count = it->data.size();
			for (int i = 0; i < count; i++)
			{
				C *c = dynamic_cast<C *>(it->data[i]);
				if (c)
				{
					c->call_shutdown();
					delete c;
					it->data[i] = nullptr;
				}
			}
		}
	}

	// create property files for all registered components in data folder ComponentSystem/
	UNIGINE_API void createPropertyFiles() const;

	// create property file for one registered component ("name" - name of the property)
	UNIGINE_API void createPropertyFile(const char *name) const;

	template <class C>
	void createPropertyFile() const
	{
		createPropertyFile(C::getPropertyName());
	}

	// rewrite property file and reload it in Property Manager
	UNIGINE_API void refreshProperty(const char *name) const;

	template <class C>
	void refreshProperty() const
	{
		refreshProperty(C::getPropertyName());
	}

	// components
	template <class C>
	C *addComponent(const NodePtr &node)
	{
		if (node.isDeleted())
			return nullptr;

		int prev_count = components[node->getID()].size();
		node->addProperty(C::getPropertyName());
		if (prev_count + 1 != components[node->getID()].size()) // add check
			return nullptr;
		return dynamic_cast<C*>(components[node->getID()][prev_count]);
	}

	template <class C>
	C *getComponent(const NodePtr &node) const
	{
		if (node.isDeleted())
			return nullptr;

		auto it = components.find(node->getID());
		if (it == components.end())
			return nullptr;

		int count = it->data.size();
		for (int i = 0; i < count; i++)
		{
			C *c = dynamic_cast<C*>(it->data[i]);
			if (c)
				return c;
		}
		return nullptr;
	}

	template <class C>
	C *getComponentInParent(const NodePtr &node) const
	{
		NodePtr n = node;
		C *c = nullptr;
		while (!c && n)
		{
			c = getComponent<C>(n);
			n = n->getParent() ? n->getParent() : n->getPossessor();
		}
		return c;
	}

	template <class C>
	C *getComponentInChildren(const NodePtr &node) const
	{
		if (node.isDeleted())
			return nullptr;

		// find in current node
		C *c = getComponent<C>(node);
		if (c)
			return c;

		// find in NodeReference
		if (node->getType() == Node::NODE_REFERENCE && !World::isUnpackNodeReferences())
		{
			NodeReferencePtr node_ref = static_ptr_cast<NodeReference>(node);
			c = getComponentInChildren<C>(node_ref->getReference());
			if (c)
				return c;
		}

		// find in nearest children
		for (int i = 0; i < node->getNumChildren(); i++)
		{
			c = getComponent<C>(node->getChild(i));
			if (c)
				return c;
		}

		// find in children of children
		for (int i = 0; i < node->getNumChildren(); i++)
		{
			c = getComponentInChildren<C>(node->getChild(i));
			if (c)
				return c;
		}

		return nullptr;
	}

	template <class C>
	void getComponents(const NodePtr &node, Vector<C*> &out_components, int clear_vector = 1) const
	{
		if (clear_vector)
			out_components.clear();

		if (node.isDeleted())
			return;

		auto it = components.find(node->getID());
		if (it == components.end())
			return;

		int count = it->data.size();
		for (int i = 0; i < count; i++)
		{
			C *c = dynamic_cast<C*>(it->data[i]);
			if (c)
				out_components.append(c);
		}
	}

	template <class C>
	void getComponentsInParent(const NodePtr &node, Vector<C*> &out_components, int clear_vector = 1) const
	{
		if (clear_vector)
			out_components.clear();

		NodePtr n = node;
		while (n)
		{
			getComponents<C>(n, out_components, 0);
			n = n->getParent() ? n->getParent() : n->getPossessor();
		}
	}

	template <class C>
	void getComponentsInChildren(const NodePtr &node, Vector<C*> &out_components, int clear_vector = 1) const
	{
		if (clear_vector)
			out_components.clear();

		if (node.isDeleted())
			return;

		// find in current node
		getComponents<C>(node, out_components, 0);

		// find in NodeReference
		if (node->getType() == Node::NODE_REFERENCE && !World::isUnpackNodeReferences())
		{
			NodeReferencePtr node_ref = static_ptr_cast<NodeReference>(node);
			getComponentsInChildren<C>(node_ref->getReference(), out_components, 0);
		}

		// find in children
		for (int i = 0; i < node->getNumChildren(); i++)
			getComponentsInChildren<C>(node->getChild(i), out_components, 0);
	}

	template <class C>
	int removeComponent(const NodePtr &node)
	{
		if (node.isNull())
			return 0;

		auto it = components.find(node->getID());
		if (it == components.end())
			return 0;

		int count = it->data.size();
		for (int i = 0; i < count; i++)
		{
			C *c = dynamic_cast<C*>(it->data[i]);
			if (c)
			{
				node->removeProperty(i);
				return 1;
			}
		}
		return 0;
	}

	// force to initialize components
	// (and doesn't wait for the next frame (WorldLogic::update() callback))
	void initializeComponents(const NodePtr &node);

	// statistics
	UNIGINE_API int getNumComponents() const;	// slow, use this method rarely
	UNIGINE_INLINE int getNumNodesWithComponents() const { return components.size(); }

	// warnings
	enum WARNING_LEVEL
	{
		NONE,
		LOW,	// when parameter types are incorrect only
		HIGH	// + when NODE/PROPERTY/MATERIAL parameters is equal to zero on startup
	};
	UNIGINE_INLINE void setWarningLevel(WARNING_LEVEL level) { warning_level = level; }
	UNIGINE_INLINE int getWarningLevel() const { return warning_level; }

	// method registators
#define METHOD_REGISTRATOR(METHOD, METHOD_LOWCASE)																						\
	UNIGINE_API void addComponentMethod##METHOD(ComponentBase *component,																\
		CallbackBase *func, const char *name = nullptr, const char *func_name = nullptr, int order = 0, bool invoke_disabled = false);	\
	UNIGINE_API void removeComponentMethod##METHOD(ComponentBase *component,															\
		CallbackBase *func, const char *name = nullptr, const char *func_name = nullptr, int order = 0, bool invoke_disabled = false);
	METHOD_REGISTRATOR(Init, init);
	METHOD_REGISTRATOR(UpdateAsyncThread, updateAsyncThread);
	METHOD_REGISTRATOR(UpdateSyncThread, updateSyncThread);
	METHOD_REGISTRATOR(Update, update);
	METHOD_REGISTRATOR(PostUpdate, postUpdate);
	METHOD_REGISTRATOR(UpdatePhysics, updatePhysics);
	METHOD_REGISTRATOR(Swap, swap);
	METHOD_REGISTRATOR(Shutdown, shutdown);
#undef METHOD_REGISTRATOR

private:
	// world's main loop
	UNIGINE_API int init() override;
	UNIGINE_API void updateAsyncThread(int id, int size) override;
	UNIGINE_API void updateSyncThread(int id, int size) override;
	UNIGINE_API int update() override;
	UNIGINE_API int postUpdate() override;
	UNIGINE_API int updatePhysics() override;
	UNIGINE_API int swap() override;
	UNIGINE_API int shutdown() override;

private:
	UNIGINE_API ComponentSystem();
	UNIGINE_API virtual ~ComponentSystem();
	ComponentSystem(ComponentSystem const&) = delete;
	ComponentSystem& operator=(ComponentSystem const&) = delete;

	// call component methods
	struct ComponentCallback;
	UNIGINE_API void run_init_methods();
	UNIGINE_API void run_thread_methods(const Map<int, Vector<ComponentCallback>> &functions, int id, int size) const;

	// callbacks
	UNIGINE_API void on_property_slots_changed(const NodePtr node, int num_slots);
	UNIGINE_API void on_property_created(const NodePtr node, const PropertyPtr prop, int num);
	UNIGINE_API void on_property_swapped(const NodePtr node, int from_num, int to_num);
	UNIGINE_API void on_property_removed(const NodePtr node, const PropertyPtr prop, int num);
	UNIGINE_API void on_property_change_enabled(const NodePtr node, const PropertyPtr prop, int num);
	UNIGINE_API void on_node_change_enabled(const NodePtr node);

	bool is_enabled = true;

	struct QueuedCallback
	{
		enum TYPE { SLOTS_CHANGED, CREATED, SWAPPED, REMOVED, CHANGE_ENABLED, NODE_CHANGE_ENABLED };
		TYPE type;
		int node;
		UGUID prop;
		int from_num;
		int num;
	};
	Vector<QueuedCallback> queued_callbacks;
	mutable Mutex callbacks_lock;

	// initialization
	Vector<CallbackBase *> initialize_callbacks;
	bool initialized;

	// profiler
	long long profiler_time;
	void render_profiler();

	// warnings
	int warning_level;

	// components with properties
	ComponentFactory component_factory;
	Map<int, Vector<ComponentBase*>> components; // <node ID, components[]>

	struct ComponentCallback
	{
		ComponentBase *component;
		CallbackBase *function;
		const char *name; // example: "ClassName::functionName", used by MicroProfiler
		const char *func_name; // example: "functionName", used to find overrides
		bool called;
		bool invoke_disabled;

		bool operator==(const ComponentCallback &other) const
		{
			return component == other.component && function == other.function;
		}
	};

#define VECTOR_OF_COMPONENT_FUNCTIONS(NAME) \
	Map<int, Vector<ComponentCallback>> component_functions_##NAME; // <order, component's functions in creation order>
	VECTOR_OF_COMPONENT_FUNCTIONS(init);
	VECTOR_OF_COMPONENT_FUNCTIONS(updateAsyncThread);
	VECTOR_OF_COMPONENT_FUNCTIONS(updateSyncThread);
	VECTOR_OF_COMPONENT_FUNCTIONS(update);
	VECTOR_OF_COMPONENT_FUNCTIONS(postUpdate);
	VECTOR_OF_COMPONENT_FUNCTIONS(updatePhysics);
	VECTOR_OF_COMPONENT_FUNCTIONS(swap);
	VECTOR_OF_COMPONENT_FUNCTIONS(shutdown);
#undef VECTOR_OF_COMPONENT_FUNCTIONS
	Map<int, Vector<ComponentCallback>> component_functions_init_delayed;
};

#ifndef __GNUC__
#pragma endregion System
#endif

////////////////////////////////////////////////////////////////////////////////////////
// Component Variables
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __GNUC__
#pragma region Variables
#endif

class ComponentVariable;
class ComponentStruct;

// class that stores info about property, its parameters and structs
class UNIGINE_CS PropertyWrapper : public Base
{
public:
	PropertyWrapper() = default;
	PropertyWrapper(const PropertyPtr &prop);
	virtual ~PropertyWrapper() = default;

	// pointer to the property
	PropertyPtr property;

	// parameter wrappers
	Vector<ComponentVariable *> variables;

	// struct wrappers
	struct ComponentStructInfo
	{
		String name;
		ComponentStruct *instance;
		ComponentVariable *variable;
	};
	Vector<ComponentStructInfo> structs;

	// helper methods
	UNIGINE_API int findStructIndex(const char *name) const;
};

class UNIGINE_CS ComponentVariable : public Base
{
public:
	ComponentVariable() = default;
	UNIGINE_API ComponentVariable(PropertyWrapper *component, const char *name, int type,
		const char *title, const char *tooltip, const char *group, const char *args);
	UNIGINE_API virtual ~ComponentVariable();

	UNIGINE_INLINE int getID() const { return parameter->getID(); }
	UNIGINE_INLINE const char *getName() const { return name; }
	UNIGINE_INLINE int getType() const { return type; }
	UNIGINE_API const char *getTypeName() const;
	virtual String getValueAsString() const { return String::null; }
	virtual int nullCheck() const { return 0; }

	UNIGINE_API virtual void save(const XmlPtr &parameter) const;

	UNIGINE_API virtual void setParameter(const PropertyParameterPtr &param);
	UNIGINE_INLINE const PropertyParameterPtr &getParameter() const { return parameter; }

protected:
	UNIGINE_API int is_type_name(const char *name) const;

	PropertyWrapper *holder = nullptr;

	PropertyParameterPtr parameter;
	const char *name = "";
	const char *title = "";
	const char *tooltip = "";
	const char *group = "";
	const char *args = "";
	int type;
};

class UNIGINE_CS ComponentVariableInt : public ComponentVariable
{
public:
	ComponentVariableInt() = default;
	UNIGINE_API ComponentVariableInt(PropertyWrapper* component, const char *name, int default_value = 0,
		const char *title = nullptr, const char *tooltip = nullptr, const char *group = nullptr, const char *args = nullptr)
		: ComponentVariable(component, name, Property::PARAMETER_INT, title, tooltip, group, args), value(default_value) {}
	virtual ~ComponentVariableInt() = default;

	UNIGINE_API ComponentVariableInt &operator=(int value);
	UNIGINE_API operator int() const;
	UNIGINE_API int get() const;
	virtual String getValueAsString() const override { return String::itoa(value); }

protected:
	int value;
};

class UNIGINE_CS ComponentVariableToggle : public ComponentVariable
{
public:
	ComponentVariableToggle() = default;
	UNIGINE_API ComponentVariableToggle(PropertyWrapper* component, const char *name, int default_value = 0,
		const char *title = nullptr, const char *tooltip = nullptr, const char *group = nullptr, const char *args = nullptr)
		: ComponentVariable(component, name, Property::PARAMETER_TOGGLE, title, tooltip, group, args), value(default_value) {}
	virtual ~ComponentVariableToggle() = default;

	UNIGINE_API ComponentVariableToggle &operator=(int value);
	UNIGINE_API operator int() const;
	UNIGINE_API int get() const;
	virtual String getValueAsString() const override { return String::itoa(value); }

protected:
	int value;
};

class UNIGINE_CS ComponentVariableSwitch : public ComponentVariable
{
public:
	ComponentVariableSwitch() = default;
	UNIGINE_API ComponentVariableSwitch(PropertyWrapper* component, const char *name, int default_value = 0, const char *in_items = nullptr,
		const char *title = nullptr, const char *tooltip = nullptr, const char *group = nullptr, const char *args = nullptr)
		: ComponentVariable(component, name, Property::PARAMETER_SWITCH, title, tooltip, group, args), value(default_value), items(in_items) {}
	virtual ~ComponentVariableSwitch() = default;

	UNIGINE_API ComponentVariableSwitch &operator=(int value);
	UNIGINE_API operator int() const;
	UNIGINE_API int get() const;
	virtual String getValueAsString() const override { return String::itoa(value); }
	UNIGINE_INLINE const char * getItems() const { return items; }

	UNIGINE_API void save(const XmlPtr &parameter) const override;

protected:
	int value;
	const char * items = nullptr;
};

class UNIGINE_CS ComponentVariableMask : public ComponentVariable
{
public:
	ComponentVariableMask() = default;
	UNIGINE_API ComponentVariableMask(PropertyWrapper* component, const char *name, int default_value = 0,
		const char *title = nullptr, const char *tooltip = nullptr, const char *group = nullptr, const char *args = nullptr)
		: ComponentVariable(component, name, Property::PARAMETER_MASK, title, tooltip, group, args), value(default_value) {}
	UNIGINE_API ComponentVariableMask(PropertyWrapper* component, const char *name, const char *mask_type, int default_value = 0,
		const char *title = nullptr, const char *tooltip = nullptr, const char *group = nullptr, const char *args = nullptr)
		: ComponentVariable(component, name, Property::PARAMETER_MASK, title, tooltip, group, args), value(default_value), flags(mask_type) {}
	virtual ~ComponentVariableMask() = default;

	UNIGINE_API ComponentVariableMask &operator=(int value);
	UNIGINE_API operator int() const;
	UNIGINE_API int get() const;
	virtual String getValueAsString() const override { return String::itoa(value); }
	UNIGINE_INLINE const char * getFlags() const { return flags; }

	UNIGINE_API void save(const XmlPtr &parameter) const override;

protected:
	int value;
	const char * flags = nullptr;
};

class UNIGINE_CS ComponentVariableFloat : public ComponentVariable
{
public:
	ComponentVariableFloat() = default;
	UNIGINE_API ComponentVariableFloat(PropertyWrapper* component, const char *name, float default_value = 0,
		const char *title = nullptr, const char *tooltip = nullptr, const char *group = nullptr, const char *args = nullptr)
		: ComponentVariable(component, name, Property::PARAMETER_FLOAT, title, tooltip, group, args), value(default_value) {}
	virtual ~ComponentVariableFloat() = default;

	UNIGINE_API ComponentVariableFloat &operator=(float value);
	UNIGINE_API operator float() const;
	UNIGINE_API float get() const;
	virtual String getValueAsString() const override { return String::ftoa(value); }

protected:
	float value;
};

class UNIGINE_CS ComponentVariableDouble : public ComponentVariable
{
public:
	ComponentVariableDouble() = default;
	UNIGINE_API ComponentVariableDouble(PropertyWrapper* component, const char *name, double default_value = 0,
		const char *title = nullptr, const char *tooltip = nullptr, const char *group = nullptr, const char *args = nullptr)
		: ComponentVariable(component, name, Property::PARAMETER_DOUBLE, title, tooltip, group, args), value(default_value) {}
	virtual ~ComponentVariableDouble() = default;

	UNIGINE_API ComponentVariableDouble &operator=(double value);
	UNIGINE_API operator double() const;
	UNIGINE_API double get() const;
	virtual String getValueAsString() const override { return String::dtoa(value); }

protected:
	double value;
};

class UNIGINE_CS ComponentVariableString : public ComponentVariable
{
public:
	ComponentVariableString() = default;
	UNIGINE_API ComponentVariableString(PropertyWrapper* component, const char *name, const char *default_value = nullptr,
		const char *title = nullptr, const char *tooltip = nullptr, const char *group = nullptr, const char *args = nullptr)
		: ComponentVariable(component, name, Property::PARAMETER_STRING, title, tooltip, group, args), value(default_value) {}
	virtual ~ComponentVariableString() = default;

	UNIGINE_API ComponentVariableString &operator=(const char *value);
	UNIGINE_API operator const char *() const;
	UNIGINE_API const char *get() const;
	virtual String getValueAsString() const override { return value; }

protected:
	mutable String value;
};

class UNIGINE_CS ComponentVariableVec2 : public ComponentVariable
{
public:
	ComponentVariableVec2() = default;
	UNIGINE_API ComponentVariableVec2(PropertyWrapper* component, const char *name,
		const char *title = nullptr, const char *tooltip = nullptr, const char *group = nullptr, const char *args = nullptr)
		: ComponentVariable(component, name, Property::PARAMETER_VEC2, title, tooltip, group, args) {}
	UNIGINE_API ComponentVariableVec2(PropertyWrapper* component, const char *name, const Math::vec2 &default_value,
		const char *title = nullptr, const char *tooltip = nullptr, const char *group = nullptr, const char *args = nullptr)
		: ComponentVariable(component, name, Property::PARAMETER_VEC2, title, tooltip, group, args), value(default_value) {}
	virtual ~ComponentVariableVec2() = default;

	UNIGINE_API ComponentVariableVec2 &operator=(const Math::vec2 &value);
	UNIGINE_API operator Math::vec2() const;
	UNIGINE_API Math::vec2 get() const;
	virtual String getValueAsString() const override { return String::format("%f %f", value.x, value.y); }

protected:
	Math::vec2 value;
};

class UNIGINE_CS ComponentVariableVec3 : public ComponentVariable
{
public:
	ComponentVariableVec3() = default;
	UNIGINE_API ComponentVariableVec3(PropertyWrapper* component, const char *name,
		const char *title = nullptr, const char *tooltip = nullptr, const char *group = nullptr, const char *args = nullptr)
		: ComponentVariable(component, name, Property::PARAMETER_VEC3, title, tooltip, group, args) {}
	UNIGINE_API ComponentVariableVec3(PropertyWrapper* component, const char *name, const Math::vec3 &default_value,
		const char *title = nullptr, const char *tooltip = nullptr, const char *group = nullptr, const char *args = nullptr)
		: ComponentVariable(component, name, Property::PARAMETER_VEC3, title, tooltip, group, args), value(default_value) {}
	virtual ~ComponentVariableVec3() = default;

	UNIGINE_API ComponentVariableVec3 &operator=(const Math::vec3 &value);
	UNIGINE_API operator Math::vec3() const;
	UNIGINE_API Math::vec3 get() const;
	virtual String getValueAsString() const override { return String::format("%f %f %f", value.x, value.y, value.z); }

protected:
	Math::vec3 value;
};

class UNIGINE_CS ComponentVariableVec4 : public ComponentVariable
{
public:
	ComponentVariableVec4() = default;
	UNIGINE_API ComponentVariableVec4(PropertyWrapper* component, const char *name,
		const char *title = nullptr, const char *tooltip = nullptr, const char *group = nullptr, const char *args = nullptr)
		: ComponentVariable(component, name, Property::PARAMETER_VEC4, title, tooltip, group, args) {}
	UNIGINE_API ComponentVariableVec4(PropertyWrapper* component, const char *name, const Math::vec4 &default_value,
		const char *title = nullptr, const char *tooltip = nullptr, const char *group = nullptr, const char *args = nullptr)
		: ComponentVariable(component, name, Property::PARAMETER_VEC4, title, tooltip, group, args), value(default_value) {}
	virtual ~ComponentVariableVec4() = default;

	UNIGINE_API ComponentVariableVec4 &operator=(const Math::vec4 &value);
	UNIGINE_API operator Math::vec4() const;
	UNIGINE_API Math::vec4 get() const;
	virtual String getValueAsString() const override { return String::format("%f %f %f %f", value.x, value.y, value.z, value.w); }

protected:
	Math::vec4 value;
};

class UNIGINE_CS ComponentVariableDVec2 : public ComponentVariable
{
public:
	ComponentVariableDVec2() = default;
	UNIGINE_API ComponentVariableDVec2(PropertyWrapper* component, const char *name,
		const char *title = nullptr, const char *tooltip = nullptr, const char *group = nullptr, const char *args = nullptr)
		: ComponentVariable(component, name, Property::PARAMETER_DVEC2, title, tooltip, group, args) {}
	UNIGINE_API ComponentVariableDVec2(PropertyWrapper* component, const char *name, const Math::dvec2 &default_value,
		const char *title = nullptr, const char *tooltip = nullptr, const char *group = nullptr, const char *args = nullptr)
		: ComponentVariable(component, name, Property::PARAMETER_DVEC2, title, tooltip, group, args), value(default_value) {}
	virtual ~ComponentVariableDVec2() = default;

	UNIGINE_API ComponentVariableDVec2 &operator=(const Math::dvec2 &value);
	UNIGINE_API operator Math::dvec2() const;
	UNIGINE_API Math::dvec2 get() const;
	virtual String getValueAsString() const override { return String::format("%lf %lf", value.x, value.y); }

protected:
	Math::dvec2 value;
};

class UNIGINE_CS ComponentVariableDVec3 : public ComponentVariable
{
public:
	ComponentVariableDVec3() = default;
	UNIGINE_API ComponentVariableDVec3(PropertyWrapper* component, const char *name,
		const char *title = nullptr, const char *tooltip = nullptr, const char *group = nullptr, const char *args = nullptr)
		: ComponentVariable(component, name, Property::PARAMETER_DVEC3, title, tooltip, group, args) {}
	UNIGINE_API ComponentVariableDVec3(PropertyWrapper* component, const char *name, const Math::dvec3 &default_value,
		const char *title = nullptr, const char *tooltip = nullptr, const char *group = nullptr, const char *args = nullptr)
		: ComponentVariable(component, name, Property::PARAMETER_DVEC3, title, tooltip, group, args), value(default_value) {}
	virtual ~ComponentVariableDVec3() = default;

	UNIGINE_API ComponentVariableDVec3 &operator=(const Math::dvec3 &value);
	UNIGINE_API operator Math::dvec3() const;
	UNIGINE_API Math::dvec3 get() const;
	virtual String getValueAsString() const override { return String::format("%lf %lf %lf", value.x, value.y, value.z); }

protected:
	Math::dvec3 value;
};

class UNIGINE_CS ComponentVariableDVec4 : public ComponentVariable
{
public:
	ComponentVariableDVec4() = default;
	UNIGINE_API ComponentVariableDVec4(PropertyWrapper* component, const char *name,
		const char *title = nullptr, const char *tooltip = nullptr, const char *group = nullptr, const char *args = nullptr)
		: ComponentVariable(component, name, Property::PARAMETER_DVEC4, title, tooltip, group, args) {}
	UNIGINE_API ComponentVariableDVec4(PropertyWrapper* component, const char *name, const Math::dvec4 &default_value,
		const char *title = nullptr, const char *tooltip = nullptr, const char *group = nullptr, const char *args = nullptr)
		: ComponentVariable(component, name, Property::PARAMETER_DVEC4, title, tooltip, group, args), value(default_value) {}
	virtual ~ComponentVariableDVec4() = default;

	UNIGINE_API ComponentVariableDVec4 &operator=(const Math::dvec4 &value);
	UNIGINE_API operator Math::dvec4() const;
	UNIGINE_API Math::dvec4 get() const;
	virtual String getValueAsString() const override { return String::format("%lf %lf %lf %lf", value.x, value.y, value.z, value.w); }

protected:
	Math::dvec4 value;
};

class UNIGINE_CS ComponentVariableIVec2 : public ComponentVariable
{
public:
	ComponentVariableIVec2() = default;
	UNIGINE_API ComponentVariableIVec2(PropertyWrapper* component, const char *name,
		const char *title = nullptr, const char *tooltip = nullptr, const char *group = nullptr, const char *args = nullptr)
		: ComponentVariable(component, name, Property::PARAMETER_IVEC2, title, tooltip, group, args) {}
	UNIGINE_API ComponentVariableIVec2(PropertyWrapper* component, const char *name, const Math::ivec2 &default_value,
		const char *title = nullptr, const char *tooltip = nullptr, const char *group = nullptr, const char *args = nullptr)
		: ComponentVariable(component, name, Property::PARAMETER_IVEC2, title, tooltip, group, args), value(default_value) {}
	virtual ~ComponentVariableIVec2() = default;

	UNIGINE_API ComponentVariableIVec2 &operator=(const Math::ivec2 &value);
	UNIGINE_API operator Math::ivec2() const;
	UNIGINE_API Math::ivec2 get() const;
	virtual String getValueAsString() const override { return String::format("%d %d", value.x, value.y); }

protected:
	Math::ivec2 value;
};

class UNIGINE_CS ComponentVariableIVec3 : public ComponentVariable
{
public:
	ComponentVariableIVec3() = default;
	UNIGINE_API ComponentVariableIVec3(PropertyWrapper* component, const char *name,
		const char *title = nullptr, const char *tooltip = nullptr, const char *group = nullptr, const char *args = nullptr)
		: ComponentVariable(component, name, Property::PARAMETER_IVEC3, title, tooltip, group, args) {}
	UNIGINE_API ComponentVariableIVec3(PropertyWrapper* component, const char *name, const Math::ivec3 &default_value,
		const char *title = nullptr, const char *tooltip = nullptr, const char *group = nullptr, const char *args = nullptr)
		: ComponentVariable(component, name, Property::PARAMETER_IVEC3, title, tooltip, group, args), value(default_value) {}
	virtual ~ComponentVariableIVec3() = default;

	UNIGINE_API ComponentVariableIVec3 &operator=(const Math::ivec3 &value);
	UNIGINE_API operator Math::ivec3() const;
	UNIGINE_API Math::ivec3 get() const;
	virtual String getValueAsString() const override { return String::format("%d %d %d", value.x, value.y, value.z); }

protected:
	Math::ivec3 value;
};

class UNIGINE_CS ComponentVariableIVec4 : public ComponentVariable
{
public:
	ComponentVariableIVec4() = default;
	UNIGINE_API ComponentVariableIVec4(PropertyWrapper* component, const char *name,
		const char *title = nullptr, const char *tooltip = nullptr, const char *group = nullptr, const char *args = nullptr)
		: ComponentVariable(component, name, Property::PARAMETER_IVEC4, title, tooltip, group, args) {}
	UNIGINE_API ComponentVariableIVec4(PropertyWrapper* component, const char *name, const Math::ivec4 &default_value,
		const char *title = nullptr, const char *tooltip = nullptr, const char *group = nullptr, const char *args = nullptr)
		: ComponentVariable(component, name, Property::PARAMETER_IVEC4, title, tooltip, group, args), value(default_value) {}
	virtual ~ComponentVariableIVec4() = default;

	UNIGINE_API ComponentVariableIVec4 &operator=(const Math::ivec4 &value);
	UNIGINE_API operator Math::ivec4() const;
	UNIGINE_API Math::ivec4 get() const;
	virtual String getValueAsString() const override { return String::format("%d %d %d %d", value.x, value.y, value.z, value.w); }

protected:
	Math::ivec4 value;
};

class UNIGINE_CS ComponentVariableColor : public ComponentVariable
{
public:
	ComponentVariableColor() = default;
	UNIGINE_API ComponentVariableColor(PropertyWrapper* component, const char *name,
		const char *title = nullptr, const char *tooltip = nullptr, const char *group = nullptr, const char *args = nullptr)
		: ComponentVariable(component, name, Property::PARAMETER_COLOR, title, tooltip, group, args) {}
	UNIGINE_API ComponentVariableColor(PropertyWrapper* component, const char *name, const Math::vec4 &default_value,
		const char *title = nullptr, const char *tooltip = nullptr, const char *group = nullptr, const char *args = nullptr)
		: ComponentVariable(component, name, Property::PARAMETER_COLOR, title, tooltip, group, args), value(default_value) {}
	virtual ~ComponentVariableColor() = default;

	UNIGINE_API ComponentVariableColor &operator=(const Math::vec4 &value);
	UNIGINE_API operator Math::vec4() const;
	UNIGINE_API Math::vec4 get() const;
	virtual String getValueAsString() const override { return String::format("%f %f %f %f", value.x, value.y, value.z, value.w); }

protected:
	Math::vec4 value;
};

class UNIGINE_CS ComponentVariableFile : public ComponentVariable
{
public:
	ComponentVariableFile() = default;
	UNIGINE_API ComponentVariableFile(PropertyWrapper* component, const char *name, const char *default_value = nullptr,
		const char *title = nullptr, const char *tooltip = nullptr, const char *group = nullptr, const char *args = nullptr)
		: ComponentVariable(component, name, Property::PARAMETER_FILE, title, tooltip, group, args), value(default_value) {}
	virtual ~ComponentVariableFile() = default;

	UNIGINE_API ComponentVariableFile &operator=(const char *value);
	UNIGINE_API operator const char*() const;
	UNIGINE_API const char *get() const;
	UNIGINE_API const char *getRaw() const;
	virtual String getValueAsString() const override { return value; }
	virtual int nullCheck() const override { return parameter ? (!parameter->isFileExist()) : 0; }

protected:
	mutable String value;
};

class UNIGINE_CS ComponentVariableProperty : public ComponentVariable
{
public:
	ComponentVariableProperty() = default;
	UNIGINE_API ComponentVariableProperty(PropertyWrapper* component, const char *name,
		const char *title = nullptr, const char *tooltip = nullptr, const char *group = nullptr, const char *args = nullptr)
		: ComponentVariable(component, name, Property::PARAMETER_PROPERTY, title, tooltip, group, args) {}
	UNIGINE_API ComponentVariableProperty(PropertyWrapper* component, const char *name, const PropertyPtr &default_value,
		const char *title = nullptr, const char *tooltip = nullptr, const char *group = nullptr, const char *args = nullptr)
		: ComponentVariable(component, name, Property::PARAMETER_PROPERTY, title, tooltip, group, args)
		, value(default_value)
		, value_guid(default_value->getGUID()) {}

	virtual ~ComponentVariableProperty() = default;

	UNIGINE_API ComponentVariableProperty &operator=(const PropertyPtr &value);
	UNIGINE_API ComponentVariableProperty &operator=(const UGUID &value);
	UNIGINE_API operator PropertyPtr() const;
	UNIGINE_API operator UGUID() const;
	UNIGINE_API PropertyPtr get() const;
	UNIGINE_API Property *operator->();
	UNIGINE_INLINE int isEmpty() const { return get().get() == nullptr; }
	virtual String getValueAsString() const override { return value_guid.getString(); }
	virtual int nullCheck() const override { return isEmpty(); }

protected:
	PropertyPtr value;
	UGUID value_guid;
};

class UNIGINE_CS ComponentVariableMaterial : public ComponentVariable
{
public:
	ComponentVariableMaterial() = default;
	UNIGINE_API ComponentVariableMaterial(PropertyWrapper* component, const char *name,
		const char *title = nullptr, const char *tooltip = nullptr, const char *group = nullptr, const char *args = nullptr)
		: ComponentVariable(component, name, Property::PARAMETER_MATERIAL, title, tooltip, group, args) {}
	UNIGINE_API ComponentVariableMaterial(PropertyWrapper* component, const char *name, const MaterialPtr &default_value,
		const char *title = nullptr, const char *tooltip = nullptr, const char *group = nullptr, const char *args = nullptr)
		: ComponentVariable(component, name, Property::PARAMETER_MATERIAL, title, tooltip, group, args)
		, value(default_value)
		, value_guid(default_value->getGUID()) {}

	virtual ~ComponentVariableMaterial() = default;

	UNIGINE_API ComponentVariableMaterial &operator=(const MaterialPtr &value);
	UNIGINE_API ComponentVariableMaterial &operator=(const UGUID &value);
	UNIGINE_API operator MaterialPtr() const;
	UNIGINE_API operator UGUID() const;
	UNIGINE_API MaterialPtr get() const;
	UNIGINE_API Material *operator->();
	UNIGINE_INLINE int isEmpty() const { return get().get() == nullptr; }
	virtual String getValueAsString() const override { return value_guid.getString(); }
	virtual int nullCheck() const override { return isEmpty(); }

protected:
	MaterialPtr value;
	UGUID value_guid;
};

class UNIGINE_CS ComponentVariableNode : public ComponentVariable
{
public:
	ComponentVariableNode() = default;
	UNIGINE_API ComponentVariableNode(PropertyWrapper* component, const char *name,
		const char *title = nullptr, const char *tooltip = nullptr, const char *group = nullptr, const char *args = nullptr)
		: ComponentVariable(component, name, Property::PARAMETER_NODE, title, tooltip, group, args)
		, value_id(0) {}
	UNIGINE_API ComponentVariableNode(PropertyWrapper* component, const char *name, const NodePtr &default_value,
		const char *title = nullptr, const char *tooltip = nullptr, const char *group = nullptr, const char *args = nullptr)
		: ComponentVariable(component, name, Property::PARAMETER_NODE, title, tooltip, group, args)
		, value(default_value)
		, value_id(default_value->getID()) {}

	virtual ~ComponentVariableNode() = default;

	UNIGINE_API ComponentVariableNode &operator=(const NodePtr &value);
	UNIGINE_API ComponentVariableNode &operator=(int value);
	UNIGINE_API operator NodePtr() const;
	UNIGINE_API operator int() const;
	UNIGINE_API NodePtr get() const;
	UNIGINE_API Node *operator->();
	UNIGINE_INLINE int isEmpty() const { return get().get() == nullptr; }
	virtual String getValueAsString() const override { return String::itoa(value_id); }
	virtual int nullCheck() const override { return isEmpty(); }

protected:
	NodePtr value;
	int value_id;
};

class UNIGINE_CS ComponentVariableCurve2d : public ComponentVariable
{
public:
	ComponentVariableCurve2d()
	{
		value->addKey({0.0f, 0.0f});
		value->addKey({1.0f, 1.0f});
	}

	UNIGINE_API ComponentVariableCurve2d(PropertyWrapper *component, const char *name,
		const char *title = nullptr, const char *tooltip = nullptr, const char *group = nullptr, const char *args = nullptr)
		: ComponentVariable(component, name, Unigine::Property::PARAMETER_CURVE2D, title, tooltip, group, args) {}
	ComponentVariableCurve2d(PropertyWrapper* component, const char *name, const Unigine::Curve2dPtr &default_value,
		const char *title = nullptr, const char *tooltip = nullptr, const char *group = nullptr, const char *args = nullptr)
		: ComponentVariable(component, name, Unigine::Property::PARAMETER_CURVE2D, title, tooltip, group, args)
		, value(default_value) {}

	virtual ~ComponentVariableCurve2d() = default;

	UNIGINE_API ComponentVariableCurve2d &operator=(const Unigine::Curve2dPtr &value);
	UNIGINE_API operator Unigine::Curve2dPtr() const;
	UNIGINE_API Unigine::Curve2dPtr get() const;
	UNIGINE_API Unigine::Curve2d *operator->();
	UNIGINE_INLINE int isEmpty() const { return get().get() == nullptr; }
	virtual Unigine::String getValueAsString() const override { return Unigine::String(""); }
	virtual int nullCheck() const override { return isEmpty(); }

protected:
	mutable Unigine::Curve2dPtr value{Curve2d::create()};
};

class UNIGINE_CS ComponentVariableStructBase : public ComponentVariable
{
public:
	ComponentVariableStructBase() = default;
	UNIGINE_API ComponentVariableStructBase(PropertyWrapper *component, const char *type_name,
		const char *name = nullptr, const char *title = nullptr, const char *tooltip = nullptr, const char *group = nullptr, const char *args = nullptr)
		: ComponentVariable(component, name, Property::PARAMETER_STRUCT, title, tooltip, group, args)
	{
		UNIGINE_UNUSED(type_name);
	}
	virtual ~ComponentVariableStructBase() = default;

	UNIGINE_INLINE ComponentStruct *getBase() const { return value_base; }
	UNIGINE_INLINE const String &getStructName() const { return struct_type; }

	UNIGINE_API void save(const XmlPtr &parameter) const override;
	UNIGINE_API void setParameter(const PropertyParameterPtr &param) override;

protected:
	String struct_type;
	ComponentStruct *value_base;
};

template <class C>
class UNIGINE_CS ComponentVariableStruct : public ComponentVariableStructBase
{
public:
	ComponentVariableStruct() = default;
	ComponentVariableStruct(PropertyWrapper *component, const char *type_name,
		const char *name = nullptr, const char *title = nullptr, const char *tooltip = nullptr, const char *group = nullptr, const char *args = nullptr)
		: ComponentVariableStructBase(component, type_name, name, title, tooltip, group, args)
	{
		// set base variables
		value_base = &value;
		struct_type = type_name;

		// set parameters to all children recursively
		setParameter(parameter);

		// add new struct to the Component
		int index = component->findStructIndex(struct_type.get());
		if (index == -1)
			component->structs.append({ struct_type /*name*/, value_base, nullptr });
	}
	virtual ~ComponentVariableStruct() = default;

	UNIGINE_INLINE C *operator->() { return &value; }
	UNIGINE_INLINE operator C() const { return value; }
	UNIGINE_INLINE C &get() { return value; }

protected:
	C value;
};

template <class C>
class UNIGINE_CS ComponentVariableArray : public ComponentVariable
{
public:
	UNIGINE_API ComponentVariableArray(): struct_reference(nullptr) {}
	ComponentVariableArray(PropertyWrapper *component, const char *name, const char *type_name,
		const char *title = nullptr, const char *tooltip = nullptr, const char *group = nullptr, const char *args = nullptr)
		: ComponentVariable(component, name, Property::PARAMETER_ARRAY, title, tooltip, group, args)
		, struct_reference(nullptr)
	{
		is_basic_type = is_type_name(type_name);

		// all basic types ("int", "float") have lower characters instead of structs ("MyStruct")
		// so, we modify the name if we got "Int" or "Float" string in type_name
		value_type = type_name;
		if (is_basic_type)
			value_type.lower();

		// add new struct to the Component
		if (!is_basic_type)
		{
			int index = component->findStructIndex(value_type.get());
			if (index == -1)
			{
				struct_reference = new C { component, type_name };
				component->variables.removeLast(); // remove reference from component (it's added in ComponentVariable constructor)
				ComponentVariableStructBase *struct_reference_base = dynamic_cast<ComponentVariableStructBase*>(struct_reference);
				component->structs.append({ value_type /*name*/, struct_reference_base->getBase(), struct_reference });
			}
		}
	}

	virtual ~ComponentVariableArray()
	{
		delete struct_reference;
	}

	void resize(int size)
	{
		int prev_size = value.size();

		// destroy old values
		for (int i = size; i < prev_size; i++)
			delete value[i];

		// resize
		value.resize(size);
		if (parameter)
			parameter->setArraySize(size);

		// initialize new values
		for (int i = prev_size; i < size; i++)
		{
			if (is_basic_type)
				value[i] = new C { holder, "" };
			else
				value[i] = new C { holder, value_type.get() };
			holder->variables.removeLast(); // remove array values from component (they are added in ComponentVariable constructor)
		}

		// set actual PropertyParameter to children
		if (parameter)
			for (int i = 0; i < size; i++)
				value[i]->setParameter(parameter->getChild(i));
	}

	UNIGINE_INLINE int size() { refresh(); return (int)value.size(); }
	UNIGINE_INLINE C &get(int index) { refresh(); return *value[index]; }
	UNIGINE_INLINE C &operator[](int index) { refresh(); return *value[index]; }

	virtual void save(const XmlPtr &xml) const override
	{
		ComponentVariable::save(xml);
		xml->setArg("array_type", value_type.get());
	}

protected:
	String value_type;
	int is_basic_type;
	Vector<C*> value;
	C *struct_reference;

private:
	// refresh links from Property to Vector<C> value variable
	void refresh()
	{
		if (parameter && parameter->getArraySize() != value.size())
			resize(parameter->getArraySize());
	}
};

#ifndef __GNUC__
#pragma endregion Variables
#endif

#ifndef __GNUC__
#pragma region VariableArguments
#endif

struct UNIGINE_CS ComponentVariableArgumentTitle
{
	UNIGINE_API ComponentVariableArgumentTitle(const char *text) { value = text; }
	static const char *value;
};

struct UNIGINE_CS ComponentVariableArgumentTooltip
{
	UNIGINE_API ComponentVariableArgumentTooltip(const char *text) { value = text; }
	static const char *value;
};

struct UNIGINE_CS ComponentVariableArgumentGroup
{
	UNIGINE_API ComponentVariableArgumentGroup(const char *text) { value = text; }
	static const char *value;
};

#ifndef __GNUC__
#pragma endregion VariableArguments
#endif

////////////////////////////////////////////////////////////////////////////////////////
// Component Bases
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __GNUC__
#pragma region Base
#endif

// base class for the components
class UNIGINE_CS ComponentBase: public PropertyWrapper
{
public:
	UNIGINE_API ComponentBase(const NodePtr &node, int num);
	UNIGINE_API virtual ~ComponentBase();

	virtual const char *getClassName() const { return "ComponentBase"; }
	virtual const char *getComponentDescription() const { return nullptr; }

	// property
	UNIGINE_INLINE static const char *getPropertyName() { return "component_base"; }
	virtual const char *getParentPropertyName() const { return "node_base"; }
	virtual int isAutoSaveProperty() const { return 1; }

	// common functions
	UNIGINE_INLINE const NodePtr &getNode() const { return node; }
	UNIGINE_INLINE int getPropertyNum() const { return property_num; }
	UNIGINE_INLINE const PropertyPtr &getProperty() const { return property; }

	UNIGINE_INLINE void setEnabled(bool enable) { return node->setPropertyEnabled(property_num, enable); }
	UNIGINE_INLINE int isEnabled() const { return enabled; }

	UNIGINE_INLINE int isInitialized() const { return all_init_called; }
	virtual bool isBase() const { return true; }

	// callbacks
	UNIGINE_INLINE void setDestroyCallback(CallbackBase *func) { delete destroy_callback; destroy_callback = func; }
	UNIGINE_INLINE void clearDestroyCallback() { delete destroy_callback; destroy_callback = nullptr; }

protected:
	friend class ComponentSystem;
	friend class ComponentVariable;

	//////////////////////////////////////////////////////////
	// virtual
	//////////////////////////////////////////////////////////

	// callbacks
	virtual void on_enable() {}		// call once when property/node was enabled
	virtual void on_disable() {}	// ... was disabled
	virtual void on_ready() {}		// call once when component was created

	//////////////////////////////////////////////////////////
	// common
	//////////////////////////////////////////////////////////

	// check XMLs equals
	UNIGINE_API int is_equals(const XmlPtr &xml1, const XmlPtr &xml2) const;

	// saves all variables to .prop file
	UNIGINE_API void save_property(const char *name) const;

	// components
	template <class C>
	C *addComponent(const NodePtr &node)
	{
		return ComponentSystem::get()->addComponent<C>(node);
	}

	template <class C>
	C *getComponent(const NodePtr &node) const
	{
		return ComponentSystem::get()->getComponent<C>(node);
	}

	template <class C>
	C *getComponentInParent(const NodePtr &node) const
	{
		return ComponentSystem::get()->getComponentInParent<C>(node);
	}

	template <class C>
	C *getComponentInChildren(const NodePtr &node) const
	{
		return ComponentSystem::get()->getComponentInChildren<C>(node);
	}

	template <class C>
	void getComponents(const NodePtr &node, Vector<C*> &components) const
	{
		ComponentSystem::get()->getComponents<C>(node, components);
	}

	template <class C>
	void getComponentsInParent(const NodePtr &node, Vector<C*> &components) const
	{
		ComponentSystem::get()->getComponentsInParent<C>(node, components);
	}

	template <class C>
	void getComponentsInChildren(const NodePtr &node, Vector<C*> &components) const
	{
		ComponentSystem::get()->getComponentsInChildren<C>(node, components);
	}

	template <class C>
	int removeComponent(const NodePtr &node)
	{
		return ComponentSystem::get()->removeComponent<C>(node);
	}

	//////////////////////////////////////////////////////////
	// variables
	//////////////////////////////////////////////////////////

	// variables
	NodePtr node;
	int property_num;

	//////////////////////////////////////////////////////////
	// functions
	//////////////////////////////////////////////////////////

#define VECTOR_OF_COMPONENT_FUNCTIONS(NAME) \
	Map<int, Vector<CallbackBase*>> functions_##NAME; // <order, component's functions>
	VECTOR_OF_COMPONENT_FUNCTIONS(init);
	VECTOR_OF_COMPONENT_FUNCTIONS(updateAsyncThread);
	VECTOR_OF_COMPONENT_FUNCTIONS(updateSyncThread);
	VECTOR_OF_COMPONENT_FUNCTIONS(update);
	VECTOR_OF_COMPONENT_FUNCTIONS(postUpdate);
	VECTOR_OF_COMPONENT_FUNCTIONS(updatePhysics);
	VECTOR_OF_COMPONENT_FUNCTIONS(swap);
	VECTOR_OF_COMPONENT_FUNCTIONS(shutdown);
#undef VECTOR_OF_COMPONENT_FUNCTIONS

private:
	// initialization
	UNIGINE_API void null_check() const;
	UNIGINE_API void initialize();
	UNIGINE_API void init_called();

	UNIGINE_API void refresh_enabled();
	UNIGINE_API void call_shutdown();

	// id of the node when component was created
	// (user can change id at runtime)
	int node_id;

	// init() method was called flag
	int initialized;
	int init_calls_count;
	int all_init_called;

	// shutdown() method was called flag
	int shutdown_called;

	int enabled_self = 0;
	int enabled = 0;
	int invoke_disabled_all_init_methods = 0;

	// callbacks
	CallbackBase *destroy_callback;
};

// base class for the structs inside components
class UNIGINE_CS ComponentStruct: public PropertyWrapper
{
public:
	ComponentStruct() = default;
	virtual ~ComponentStruct() = default;
};

#ifndef __GNUC__
#pragma endregion Base
#endif

#if defined(UNIGINE_CS)
	#undef UNIGINE_CS
#endif
} /* namespace Unigine */
