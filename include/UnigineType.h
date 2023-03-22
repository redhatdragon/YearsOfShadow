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

#ifndef DONT_USE_RTTI
	#include <typeinfo>
#endif

#include "UnigineBase.h"

/**
 * Unigine namespace.
 */
namespace Unigine {

	/*
	 */
	class Xml;
	class Gui;
	class Path;
	class Mesh;
	class Node;
	class Light;
	class Decal;
	class Object;
	class Player;
	class Image;
	class Stream;
	class Controls;
	class Material;
	class Property;
	class Widget;
	class Camera;
	class Ellipsoid;
	class Dataset;

	/**************************************************************************\
	*
	* IntToType
	*
	\**************************************************************************/

	/**
	 * IntToType template.
	 */
	template <int Value> struct IntToType {
		enum { value = Value };
	};

	/**************************************************************************\
	*
	* TypeToType
	*
	\**************************************************************************/

	/**
	 * TypeToType template.
	 */
	template <class Type> struct TypeToType {
		typedef Type type;
	};

	/**************************************************************************\
	*
	* TypeID
	*
	\**************************************************************************/

	/// @cond

	/*
	 */
	UNIGINE_API int get_type_id();

	/// @endcond

	/**
	 * TypeID template.
	 */
	template <class Type> struct TypeID {
		static int id;
		static const char *name;
	};

	/// @cond

	/*
	 */
	template <class Type> int TypeID<Type>::id = get_type_id();
	#ifdef DONT_USE_RTTI
		template <class Type> const char *TypeID<Type>::name = "unknown";
	#else
		template <class Type> const char *TypeID<Type>::name = typeid(Type).name();
	#endif

	/*
	 */
	#ifdef _WIN32
		template <> int TypeID<Xml*>::id = 0x70000000;
		template <> const char *TypeID<Xml*>::name = "Unigine::Xml*";

		template <> int TypeID<Gui*>::id = 0x70000001;
		template <> const char *TypeID<Gui*>::name = "Unigine::Gui*";

		template <> int TypeID<Path*>::id = 0x70000002;
		template <> const char *TypeID<Path*>::name = "Unigine::Path*";

		template <> int TypeID<Mesh*>::id = 0x70000003;
		template <> const char *TypeID<Mesh*>::name = "Unigine::Mesh*";

		template <> int TypeID<Node*>::id = 0x70000004;
		template <> const char *TypeID<Node*>::name = "Unigine::Node*";

		template <> int TypeID<Light*>::id = 0x70000005;
		template <> const char *TypeID<Light*>::name = "Unigine::Light*";

		template <> int TypeID<Decal*>::id = 0x70000006;
		template <> const char *TypeID<Decal*>::name = "Unigine::Decal*";

		template <> int TypeID<Object*>::id = 0x70000007;
		template <> const char *TypeID<Object*>::name = "Unigine::Object*";

		template <> int TypeID<Player*>::id = 0x70000008;
		template <> const char *TypeID<Player*>::name = "Unigine::Player*";

		template <> int TypeID<Image*>::id = 0x70000009;
		template <> const char *TypeID<Image*>::name = "Unigine::Image*";

		template <> int TypeID<Stream*>::id = 0x7000000a;
		template <> const char *TypeID<Stream*>::name = "Unigine::Stream*";

		template <> int TypeID<Controls*>::id = 0x7000000b;
		template <> const char *TypeID<Controls*>::name = "Unigine::Controls*";

		template <> int TypeID<Material*>::id = 0x7000000c;
		template <> const char *TypeID<Material*>::name = "Unigine::Material*";

		template <> int TypeID<Property*>::id = 0x7000000d;
		template <> const char *TypeID<Property*>::name = "Unigine::Property*";

		template <> int TypeID<Widget*>::id = 0x7000000e;
		template <> const char *TypeID<Widget*>::name = "Unigine::Widget*";

		template <> int TypeID<Camera*>::id = 0x7000000f;
		template <> const char *TypeID<Camera*>::name = "Unigine::Camera*";

		template <> int TypeID<Ellipsoid*>::id = 0x70000010;
		template <> const char *TypeID<Ellipsoid*>::name = "Unigine::Ellipsoid*";

		template <> int TypeID<Dataset*>::id = 0x70000011;
		template <> const char *TypeID<Dataset*>::name = "Unigine::Dataset*";
	#else
		template <> UNIGINE_API int TypeID<Xml*>::id;
		template <> UNIGINE_API const char *TypeID<Xml*>::name;

		template <> UNIGINE_API int TypeID<Gui*>::id;
		template <> UNIGINE_API const char *TypeID<Gui*>::name;

		template <> UNIGINE_API int TypeID<Path*>::id;
		template <> UNIGINE_API const char *TypeID<Path*>::name;

		template <> UNIGINE_API int TypeID<Mesh*>::id;
		template <> UNIGINE_API const char *TypeID<Mesh*>::name;

		template <> UNIGINE_API int TypeID<Node*>::id;
		template <> UNIGINE_API const char *TypeID<Node*>::name;

		template <> UNIGINE_API int TypeID<Light*>::id;
		template <> UNIGINE_API const char *TypeID<Light*>::name;

		template <> UNIGINE_API int TypeID<Decal*>::id;
		template <> UNIGINE_API const char *TypeID<Decal*>::name;

		template <> UNIGINE_API int TypeID<Object*>::id;
		template <> UNIGINE_API const char *TypeID<Object*>::name;

		template <> UNIGINE_API int TypeID<Player*>::id;
		template <> UNIGINE_API const char *TypeID<Player*>::name;

		template <> UNIGINE_API int TypeID<Image*>::id;
		template <> UNIGINE_API const char *TypeID<Image*>::name;

		template <> UNIGINE_API int TypeID<Stream*>::id;
		template <> UNIGINE_API const char *TypeID<Stream*>::name;

		template <> UNIGINE_API int TypeID<Controls*>::id;
		template <> UNIGINE_API const char *TypeID<Controls*>::name;

		template <> UNIGINE_API int TypeID<Material*>::id;
		template <> UNIGINE_API const char *TypeID<Material*>::name;

		template <> UNIGINE_API int TypeID<Property*>::id;
		template <> UNIGINE_API const char *TypeID<Property*>::name;

		template <> UNIGINE_API int TypeID<Widget*>::id;
		template <> UNIGINE_API const char *TypeID<Widget*>::name;

		template <> UNIGINE_API int TypeID<Camera*>::id;
		template <> UNIGINE_API const char *TypeID<Camera*>::name;

		template <> UNIGINE_API int TypeID<Ellipsoid*>::id;
		template <> UNIGINE_API const char *TypeID<Ellipsoid*>::name;

		template <> UNIGINE_API int TypeID<Dataset*>::id;
		template <> UNIGINE_API const char *TypeID<Dataset*>::name;
	#endif
	/// @endcond

	/******************************************************************************\
	*
	* TypeInfo
	*
	\******************************************************************************/

	/**
	 * TypeInfo RTTI class.
	 */
	class TypeInfo {

		public:

			/**
			 * Default constructor that produces an empty class.
			 */
			TypeInfo() : id(0), name(0) { }
			/**
			 * Constructor.
			 *
			 * @param id type identifier.
			 * @param name type name.
			 */
			TypeInfo(int id,const char *name) : id(id), name(name) { }
			/**
			 * Copy constructor.
			 *
			 * @param type TypeInfo class.
			 */
			TypeInfo(const TypeInfo &type) : id(type.id), name(type.name) { }
			/**
			 * Constructor.
			 *
			 * @param type TypeID class.
			 */
			template <class Type> explicit TypeInfo(Type type) : id(type.id), name(type.name) { }
			/**
			 * Destructor.
			 */
			~TypeInfo() { }

			/**
			 * Access to the type identifier.
			 *
			 * @return Type identifier.
			 */
			UNIGINE_INLINE int getID() const { return id; }
			/**
			 * Access to the type name.
			 *
			 * @return Type name.
			 */
			UNIGINE_INLINE const char *getName() const { return name; }

		private:

			/// @cond

			int id;
			const char *name;

			/// @endcond
	};

	/**
	 * Check if two TypeInfo classes are referenced the same types.
	 *
	 * @param t0 The first TypeIndo class.
	 * @param t1 The second TypeIndo class.
	 * @return Returns 1 if types are the same; otherwise, 0 is returned.
	 */
	UNIGINE_INLINE int operator==(const TypeInfo &t0,const TypeInfo &t1) {
		return (t0.getID() == t1.getID());
	}

	/**
	 * Check if two TypeInfo classes are not referenced the same types.
	 *
	 * @param t0 The first TypeIndo class.
	 * @param t1 The second TypeIndo class.
	 * @return Returns 1 if types are not the same; otherwise, 0 is returned.
	 */
	UNIGINE_INLINE int operator!=(const TypeInfo &t0,const TypeInfo &t1) {
		return (t0.getID() != t1.getID());
	}

	/**************************************************************************\
	*
	* TypeList
	*
	\**************************************************************************/

	/**
	 * TypeListEnd struct.
	 */
	struct TypeListEnd {

	};

	/**
	 * TypeList template.
	 */
	template <class T,class U> struct TypeList {
		typedef T Head;
		typedef U Tail;
	};

	/**
	 * MakeTypeList template.
	 */
	template <class T0 = TypeListEnd,class T1 = TypeListEnd,class T2 = TypeListEnd,class T3 = TypeListEnd,
	class T4 = TypeListEnd,class T5 = TypeListEnd,class T6 = TypeListEnd,class T7 = TypeListEnd,
	class T8 = TypeListEnd,class T9 = TypeListEnd,class T10 = TypeListEnd,class T11 = TypeListEnd,
	class T12 = TypeListEnd,class T13 = TypeListEnd,class T14 = TypeListEnd,class T15 = TypeListEnd>
	struct MakeTypeList {
		typedef TypeList<T0,typename MakeTypeList<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15>::Type> Type;
	};

	/// @cond

	template <> struct MakeTypeList<> {
		typedef TypeListEnd Type;
	};

	/// @endcond

	/**
	 * Length template.
	 */
	template <class T> struct Length;

	/// @cond

	template <> struct Length<TypeListEnd> {
		enum { Value = 0 };
	};

	template <class T,class U> struct Length<TypeList<T,U> > {
		enum { Value = 1 + Length<U>::Value };
	};

	/// @endcond

	/**
	 * TypeAt template.
	 */
	template <class T,unsigned int> struct TypeAt {
		typedef TypeListEnd Type;
	};

	/// @cond

	template <class T,class U> struct TypeAt<TypeList<T,U>,0> {
		typedef T Type;
	};

	template <class T,class U,unsigned int i> struct TypeAt<TypeList<T,U>,i> {
		typedef typename TypeAt<U,i-1>::Type Type;
	};

	/// @endcond

	/******************************************************************************\
	*
	* IsClassFunction
	*
	\******************************************************************************/

	/**
	 * IsClassFunction template.
	 */
	template <class Type> struct IsClassFunction {
		enum { Value = 0 };
	};

	/// @cond

	template <class Class,class Type> struct IsClassFunction<Type (Class::*)()> { enum { Value = 1 }; };
	template <class Class,class Type> struct IsClassFunction<Type (Class::*)() const> { enum { Value = 1 }; };
	template <class Class,class Type,class A0> struct IsClassFunction<Type (Class::*)(A0)> { enum { Value = 1 }; };
	template <class Class,class Type,class A0> struct IsClassFunction<Type (Class::*)(A0) const> { enum { Value = 1 }; };
	template <class Class,class Type,class A0,class A1> struct IsClassFunction<Type (Class::*)(A0,A1)> { enum { Value = 1 }; };
	template <class Class,class Type,class A0,class A1> struct IsClassFunction<Type (Class::*)(A0,A1) const> { enum { Value = 1 }; };
	template <class Class,class Type,class A0,class A1,class A2> struct IsClassFunction<Type (Class::*)(A0,A1,A2)> { enum { Value = 1 }; };
	template <class Class,class Type,class A0,class A1,class A2> struct IsClassFunction<Type (Class::*)(A0,A1,A2) const> { enum { Value = 1 }; };
	template <class Class,class Type,class A0,class A1,class A2,class A3> struct IsClassFunction<Type (Class::*)(A0,A1,A2,A3)> { enum { Value = 1 }; };
	template <class Class,class Type,class A0,class A1,class A2,class A3> struct IsClassFunction<Type (Class::*)(A0,A1,A2,A3) const> { enum { Value = 1 }; };
	template <class Class,class Type,class A0,class A1,class A2,class A3,class A4> struct IsClassFunction<Type (Class::*)(A0,A1,A2,A3,A4)> { enum { Value = 1 }; };
	template <class Class,class Type,class A0,class A1,class A2,class A3,class A4> struct IsClassFunction<Type (Class::*)(A0,A1,A2,A3,A4) const> { enum { Value = 1 }; };
	template <class Class,class Type,class A0,class A1,class A2,class A3,class A4,class A5> struct IsClassFunction<Type (Class::*)(A0,A1,A2,A3,A4,A5)> { enum { Value = 1 }; };
	template <class Class,class Type,class A0,class A1,class A2,class A3,class A4,class A5> struct IsClassFunction<Type (Class::*)(A0,A1,A2,A3,A4,A5) const> { enum { Value = 1 }; };
	template <class Class,class Type,class A0,class A1,class A2,class A3,class A4,class A5,class A6> struct IsClassFunction<Type (Class::*)(A0,A1,A2,A3,A4,A5,A6)> { enum { Value = 1 }; };
	template <class Class,class Type,class A0,class A1,class A2,class A3,class A4,class A5,class A6> struct IsClassFunction<Type (Class::*)(A0,A1,A2,A3,A4,A5,A6) const> { enum { Value = 1 }; };
	template <class Class,class Type,class A0,class A1,class A2,class A3,class A4,class A5,class A6,class A7> struct IsClassFunction<Type (Class::*)(A0,A1,A2,A3,A4,A5,A6,A7)> { enum { Value = 1 }; };
	template <class Class,class Type,class A0,class A1,class A2,class A3,class A4,class A5,class A6,class A7> struct IsClassFunction<Type (Class::*)(A0,A1,A2,A3,A4,A5,A6,A7) const> { enum { Value = 1 }; };
	template <class Class,class Type,class A0,class A1,class A2,class A3,class A4,class A5,class A6,class A7,class A8> struct IsClassFunction<Type (Class::*)(A0,A1,A2,A3,A4,A5,A6,A7,A8)> { enum { Value = 1 }; };
	template <class Class,class Type,class A0,class A1,class A2,class A3,class A4,class A5,class A6,class A7,class A8> struct IsClassFunction<Type (Class::*)(A0,A1,A2,A3,A4,A5,A6,A7,A8) const> { enum { Value = 1 }; };

	/// @endcond

} /* namespace Unigine */
