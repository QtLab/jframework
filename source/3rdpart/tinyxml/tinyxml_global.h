#ifndef TINYXML_GLOBAL_H
#define TINYXML_GLOBAL_H

// TINYXML_VERSION is (major << 16) + (minor << 8) + patch.

#define TINYXML_VERSION       0x020602
#define TINYXML_VERSION_STR   "2.6.2"

#ifdef _MSC_VER /* MSVC Compiler */
/* template-class specialization 'identifier' is already instantiated */
#pragma warning(disable: 4660)

#define _TINYXML_LIBNAME_1_	"tinyxml"

#ifdef _UNICODE
#define _TINYXML_LIBNAME_2_	_TINYXML_LIBNAME_1_##"u"
#else
#define _TINYXML_LIBNAME_2_	_TINYXML_LIBNAME_1_
#endif

#ifdef _DEBUG
#define _TINYXML_LIBNAME_3_	_TINYXML_LIBNAME_2_##"d"
#else
#define _TINYXML_LIBNAME_3_	_TINYXML_LIBNAME_2_
#endif

#define _TINYXML_LIBNAME_		_TINYXML_LIBNAME_3_##".lib"

#pragma comment(lib, _TINYXML_LIBNAME_)

#ifndef _TINYXML_HIDE_AUTOLINK_OUTPUT_
#pragma comment (lib, _TINYXML_LIBNAME_)
#pragma message("Automatically linking with " _TINYXML_LIBNAME_)
#endif	// _TINYXML_HIDE_AUTOLINK_OUTPUT_

#endif // _MSC_VER

#endif // TINYXML_GLOBAL_H
