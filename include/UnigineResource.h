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

/**
 * Unigine namespace.
 */
namespace Unigine {
	
	/**
	 * Unigine Resource.
	 */
	class Resource {
			
		public:
			
			/**
			 * Loads resource from ascii string.
			 *
			 * @param data Encoded resource ascii string.
			 * @return Pointer to the decoded data (null-terminated).
			 */
			static UNIGINE_API unsigned char *load(const char *data[]);
			/**
			 * Loads resource from ascii string.
			 *
			 * @param data Encoded resource ascii string.
			 * @param size The size of decoded data.
			 * @return Pointer to the decoded data (null-terminated).
			 */
			static UNIGINE_API unsigned char *load(const char *data[],int &size);
			/**
			 * Saves data into the ascii string.
			 *
			 * @param name The name of resource.
			 * @param data The resource data.
			 * @param size The resource size.
			 * @return Encoded resource ascii string.
			 */
			static UNIGINE_API const char *save(const char *name,const unsigned char *data,int size);
	};
	
} /* namespace Unigine */
