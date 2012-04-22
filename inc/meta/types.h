/** 
 * @file	types.h
 * @brief	types definitions.
 * @author	Darren Liew<br>
 *			Copyleft (C) 2009 RaceHazard Pte Ltd.
 *
 * types definitions
 */
#pragma once
#ifndef _PICA_TYPES_H_
#define _PICA_TYPES_H_

namespace meta {

typedef signed char			S8;      ///< Compiler independent Signed Char
typedef unsigned char		U8;      ///< Compiler independent Unsigned Char

typedef signed short		S16;     ///< Compiler independent Signed 16-bit short
typedef unsigned short		U16;     ///< Compiler independent Unsigned 16-bit short

typedef signed int			S32;     ///< Compiler independent Signed 32-bit integer
typedef unsigned int		U32;     ///< Compiler independent Unsigned 32-bit integer

typedef float				F32;     ///< Compiler independent 32-bit float
typedef double				F64;     ///< Compiler independent 64-bit float

typedef signed _int64		S64;     ///< Compiler dependent signed 64-bit integer
typedef unsigned _int64		U64;     ///< Compiler dependent unsigned 64-bit integer

} // namespace meta

#endif // _PICA_TYPES_H_

