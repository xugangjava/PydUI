#include "stdafx.h"
#include "XUIManager.h"

HINSTANCE UIManager::hInstance=NULL;
INT UIManager::iObjId=0;
HFONT UIManager::m_hFonts[XFONT__LAST]={0};
LOGFONT UIManager::m_aLogFonts[XFONT__LAST]={0};
ULONG_PTR UIManager::m_gdiplusToken=NULL;  

using namespace boost::python;

VOID UIManager::Init()
{
	UIManager::hInstance=hInstance;
	 Gdiplus::GdiplusStartupInput StartupInput;  
	::CoInitialize(NULL);
	 GdiplusStartup(&m_gdiplusToken,&StartupInput,NULL);  

	//fonts
	LOGFONT lf = { 0 };
	::GetObject(::GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
	//strcpy(lf.lfFaceName, "Tahoma");
	lf.lfHeight = -12;
	m_aLogFonts[XFONT_NORMAL] = lf;
	m_aLogFonts[XFONT_CAPTION] = lf;
	LOGFONT lfBold = lf;
	lfBold.lfWeight += FW_BOLD;
	m_aLogFonts[XFONT_BOLD] = lfBold;
	lfBold.lfHeight -= 2;
	m_aLogFonts[XFONT_TITLE] = lfBold;
	lfBold.lfHeight -= 4;
	m_aLogFonts[XFONT_HEADLINE] = lfBold;
	LOGFONT lfSubscript = lf;
	lfSubscript.lfHeight -= 4;
	m_aLogFonts[XFONT_SUBSCRIPT] = lfSubscript;
	LOGFONT lfLink = lf;
	lfLink.lfUnderline = TRUE;
	m_aLogFonts[XFONT_LINK] = lfLink;

}

HFONT UIManager::GetFont(int fIndex)
{
	if(fIndex<XFONT__FIRST||fIndex>XFONT__LAST){
		return NULL;
	}
	if( m_hFonts[fIndex] == NULL ) m_hFonts[fIndex] = ::CreateFontIndirect(&m_aLogFonts[fIndex]);
	return m_hFonts[fIndex];
}

INT UIManager::NewId()
{
	UIManager::iObjId++;
	return UIManager::iObjId;
}

VOID UIManager::Dispose()
{
	GdiplusShutdown(m_gdiplusToken);  
	::CoUninitialize();
}

BOOL UIManager::TranslateMessage(const LPMSG pMsg)
{
	return FALSE;
}

VOID UIManager::MessageLoop()
{
	MSG msg = { 0 };
	while( ::GetMessage(&msg, NULL, 0, 0) ) {
		  if( !UIManager::TranslateMessage(&msg) ) {
			 ::TranslateMessage(&msg);
			 ::DispatchMessage(&msg);
		  }
	}
}



