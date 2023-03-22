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
	 * Unigine Compress.
	 */
	class Compress {
			
		public:
			
			/**
			 * Returns the maximum compressed buffer size.
			 *
			 * @param size The uncompressed data size.
			 * @return The maximum compressed buffer size.
			 */
			static UNIGINE_API size_t zlibSize(size_t size);
			static UNIGINE_API size_t lz4Size(size_t size);
			static UNIGINE_API size_t jackallessSize(size_t size);
			/**
			 * Compress the data.
			 *
			 * @param dest The destination buffer pointer.
			 * @param dest_size The destination buffer size.
			 * @param src The source data pointer.
			 * @param src_size The source data size.
			 * @param quality Compression quality; 0 is for better performance, 1 is for better size.
			 * @return Returns 1 if the operation was a success; otherwise, 0 is returned.
			 */
			static UNIGINE_API bool zlibCompress(void *dest, size_t &dest_size, const void *src, size_t src_size,bool quality);
			static UNIGINE_API bool lz4Compress(void *dest, size_t &dest_size, const void *src, size_t src_size,bool quality);
			/**
			 * Decompress the data.
			 *
			 * @param dest The destination buffer pointer.
			 * @param dest_size The destination buffer size.
			 * @param src The source data pointer.
			 * @param src_size The source data size.
			 * @return Returns 1 if the operation was a success; otherwise, 0 is returned.
			 */
			static UNIGINE_API bool zlibDecompress(void *dest, size_t dest_size, const void *src, size_t src_size);
			static UNIGINE_API bool lz4Decompress(void *dest, size_t dest_size, const void *src, size_t src_size);

			static UNIGINE_API bool jackallessCompress(void *dest, size_t &dest_size, const void *src, size_t src_size);
			static UNIGINE_API bool jackallessDecompress(void *dest, const void *src);
			static UNIGINE_API bool jackallessCompressFloat(void *dest, size_t &dest_size, const float *src, size_t src_length);
			static UNIGINE_API bool jackallessDecompressFloat(float *dest, size_t dest_length, const void *src);

			/**
			 * Returns decompressed data size
			 *
			 * @param compressed_data Compressed data pointer
			 * @return Returns size of the uncompressed data size if the operation was a success; otherwise, 0 is returned.
			 */
			static UNIGINE_API size_t jackallessRawSize(void *compressed_data);
			/**
			 * Returns compressed data size
			 *
			 * @param compressed_data Compressed data pointer
			 * @return Returns size of the compressed data size if the operation was a success; otherwise, 0 is returned.
			 */
			static UNIGINE_API size_t jackallessCompressedSize(void *compressed_data);
	};
	
} /* namespace Unigine */
