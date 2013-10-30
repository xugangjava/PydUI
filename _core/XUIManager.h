#include "stdafx.h"
#include <GdiPlus.h>  
using namespace Gdiplus;

class UIManager
{
public:
	 static HINSTANCE hInstance;
	 static INT iObjId;
	 
public:
	static VOID Init();
	static VOID Dispose();
	static INT NewId();
	static BOOL TranslateMessage(const LPMSG pMsg);
	static VOID MessageLoop();
	static HFONT GetFont(int fIndex);

private:
	static HFONT m_hFonts[XFONT__LAST];
	static LOGFONT m_aLogFonts[XFONT__LAST];

	static ULONG_PTR m_gdiplusToken;  
};