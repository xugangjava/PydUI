// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息


// Windows 头文件:
#include <windows.h>
#include <WindowsX.h>
#include <Python.h>
#include <boost/python.hpp>
#include <string>
#include <objbase.h>
#include "sqlite3.h"
typedef enum
{
	XFONT__FIRST = 0,
	XFONT_NORMAL,
	XFONT_BOLD,
	XFONT_CAPTION,
	XFONT_MENU,
	XFONT_LINK,
	XFONT_TITLE,
	XFONT_HEADLINE,
	XFONT_SUBSCRIPT,
	XFONT__LAST,
} XTYPE_FONT;

typedef enum
{
	XTYPE_DRAW__FIRST = 0,
	XTYPE_DRAW_Default,
	XTYPE_DRAW_ThreeInOneH,
	XTYPE_DRAW_ThreeInOneV,
	XTYPE_DRAW_FiveInOneH,
	XTYPE_DRAW_FiveInOneV,
	XTYPE_DRAW_NineInOne,
	XTYPE_DRAW__LAST

} XTYPE_DRAW;

#define ASSERT(expr)  _ASSERTE(expr)
