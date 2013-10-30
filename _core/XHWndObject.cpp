// PydUI.cpp : 定义 DLL 应用程序的导出函数。
//
#include "stdafx.h"
#include "XHWndObject.h"
#include "XUIManager.h"
#include "XLock.h"

using namespace ::boost::python;


int HWndObject::Create(
						int hwndParent, LPCTSTR pstrName, int dwStyle,
						int dwExStyle, int x, int y, 
						int cx, int cy, int hMenu)
{
   if(!m_sSubClassName.empty()&&!RegisterSuperclass() ) return NULL;
   if(m_sSubClassName.empty()&& !RegisterWindowClass() ) return NULL;
   m_hWnd = ::CreateWindowEx(
	   (DWORD)dwExStyle, 
	   m_sClassName.c_str(), 
	   pstrName, 
	   (DWORD)dwStyle, 
	   x, y, cx, cy,
	   (HWND)hwndParent, 
	   (HMENU)hMenu, 
	   UIManager::hInstance,
	   this);
   ASSERT(m_hWnd!=NULL);
   return (int)m_hWnd;
}

LRESULT CALLBACK HWndObject::__WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   HWndObject* pThis = NULL;
   if( uMsg == WM_NCCREATE ) {
      LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
      pThis = static_cast<HWndObject*>(lpcs->lpCreateParams);
      pThis->m_hWnd = hWnd;
      ::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LPARAM>(pThis));
   } 
   else {
      pThis = reinterpret_cast<HWndObject*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
      if( uMsg == WM_NCDESTROY && pThis != NULL ) {
         LRESULT lRes = ::CallWindowProc(pThis->m_oldWndProc, hWnd, uMsg, wParam, lParam);
         ::SetWindowLongPtr(pThis->m_hWnd, GWLP_USERDATA, 0L);
         if( pThis->m_bSubclassed ) pThis->Unsubclass();
		 pThis->m_hWnd = NULL;
		 pThis->OnFinalMessage();
         return lRes;
      }
   }
   if( pThis != NULL ) {
      return pThis->HandleMessage(uMsg, wParam, lParam);
   } 
   else {
      return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
   }
}

LRESULT CALLBACK HWndObject::__ControlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   HWndObject* pThis = NULL;
   if( uMsg == WM_NCCREATE ) {
      LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
      pThis = static_cast<HWndObject*>(lpcs->lpCreateParams);
      ::SetProp(hWnd, "WndX", (HANDLE) pThis);
      pThis->m_hWnd = hWnd;
   } 
   else {
      pThis = reinterpret_cast<HWndObject*>(::GetProp(hWnd, "WndX"));
      if( uMsg == WM_NCDESTROY && pThis != NULL ) {
         LRESULT lRes = ::CallWindowProc(pThis->m_oldWndProc, hWnd, uMsg, wParam, lParam);
         if( pThis->m_bSubclassed ) pThis->Unsubclass();
         ::SetProp(hWnd, "WndX", NULL);
		 pThis->m_hWnd = NULL;
		 pThis->OnFinalMessage();
         return lRes;
      }
   }
   if( pThis != NULL ) {
      return pThis->HandleMessage(uMsg, wParam, lParam);
   } 
   else {
      return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
   }
}



BOOL HWndObject::RegisterWindowClass()
{
   WNDCLASS wc = { 0 };
   wc.style = m_iClassStyle;
   wc.cbClsExtra = 0;
   wc.cbWndExtra = 0;
   wc.hIcon = NULL;
   wc.lpfnWndProc = __WndProc;
   wc.hInstance =UIManager::hInstance;
   wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
   wc.hbrBackground = NULL;
   wc.lpszMenuName  = NULL;
   wc.lpszClassName = m_sClassName.c_str();
   ATOM ret = ::RegisterClass(&wc);
   ASSERT(ret!=NULL || ::GetLastError()==ERROR_CLASS_ALREADY_EXISTS);
   return ret != NULL || ::GetLastError() == ERROR_CLASS_ALREADY_EXISTS;
}

BOOL HWndObject::RegisterSuperclass()
{
   WNDCLASSEX wc = { 0 };
   wc.cbSize = sizeof(WNDCLASSEX);
   if( !::GetClassInfoEx(NULL, m_sSubClassName.c_str(), &wc) ) {
      if( !::GetClassInfoEx(UIManager::hInstance, m_sSubClassName.c_str(), &wc) ) {
         ASSERT(!"Unable to locate window class");
         return NULL;
      }
   }
   m_oldWndProc = wc.lpfnWndProc;
   wc.lpfnWndProc = HWndObject::__ControlProc;
   wc.hInstance = UIManager::hInstance;
   wc.lpszClassName = m_sClassName.c_str();
   ATOM ret = ::RegisterClassEx(&wc);
   ASSERT(ret!=NULL || ::GetLastError()==ERROR_CLASS_ALREADY_EXISTS);
   return ret != NULL || ::GetLastError() == ERROR_CLASS_ALREADY_EXISTS;
}



int HWndObject::Subclass(HWND sbhWnd)
{
   ASSERT(::IsWindow(sbhWnd));
   ASSERT(m_hWnd==NULL);
   m_oldWndProc = SubclassWindow(sbhWnd, __WndProc);
   if( m_oldWndProc == NULL ) return NULL;
   m_bSubclassed = true;
   m_hWnd = sbhWnd;
   return (int)m_hWnd;
}

VOID HWndObject::Unsubclass()
{
   ASSERT(::IsWindow(m_hWnd));
   if( !::IsWindow(m_hWnd) ) return;
   if( !m_bSubclassed ) return;
   SubclassWindow(m_hWnd, m_oldWndProc);
   m_oldWndProc = ::DefWindowProc;
   m_bSubclassed = false;
}


VOID HWndObject::ShowWindow(bool bShow /*= true*/, bool bTakeFocus /*= false*/)
{
   ASSERT(::IsWindow(m_hWnd));
   if( !::IsWindow(m_hWnd) ) return;
   ::ShowWindow(m_hWnd, bShow ? (bTakeFocus ? SW_SHOWNORMAL : SW_SHOWNOACTIVATE) : SW_HIDE);
}

BOOL HWndObject::ShowModal()
{
   ASSERT(::IsWindow(m_hWnd));
   HWND hWndParent = GetWindowOwner(m_hWnd);
   ::ShowWindow(m_hWnd, SW_SHOWNORMAL);
   ::EnableWindow(hWndParent, FALSE);
   MSG msg = { 0 };
   while( ::IsWindow(m_hWnd) && ::GetMessage(&msg, NULL, 0, 0) ) {
      if( msg.message == WM_CLOSE ) {
         ::EnableWindow(hWndParent, TRUE);
         ::SetFocus(hWndParent);
      }
      if( !UIManager::TranslateMessage(&msg) ) {
         ::TranslateMessage(&msg);
         ::DispatchMessage(&msg);
      }
      if( msg.message == WM_QUIT ) break;
   }
   ::EnableWindow(hWndParent, TRUE);
   ::SetFocus(hWndParent);
   if( msg.message == WM_QUIT ) ::PostQuitMessage(msg.wParam);
   return true;
}

VOID HWndObject::Close()
{
   ASSERT(::IsWindow(m_hWnd));
   if( !::IsWindow(m_hWnd) ) return;
   PostMessage(WM_CLOSE);
}

VOID HWndObject::CenterWindow()
{
   ASSERT(::IsWindow(m_hWnd));
   ASSERT((GetWindowStyle(m_hWnd)&WS_CHILD)==0);
   RECT rcDlg = { 0 };
   ::GetWindowRect(m_hWnd, &rcDlg);
   RECT rcArea = { 0 };
   RECT rcCenter = { 0 };
   HWND hWndParent = ::GetParent(m_hWnd);
   HWND hWndCenter = ::GetWindowOwner(m_hWnd);
   ::SystemParametersInfo(SPI_GETWORKAREA, NULL, &rcArea, NULL);
   if( hWndCenter == NULL ) rcCenter = rcArea; else ::GetWindowRect(hWndCenter, &rcCenter);

   int DlgWidth = rcDlg.right - rcDlg.left;
   int DlgHeight = rcDlg.bottom - rcDlg.top;

   // Find dialog's upper left based on rcCenter
   int xLeft = (rcCenter.left + rcCenter.right) / 2 - DlgWidth / 2;
   int yTop = (rcCenter.top + rcCenter.bottom) / 2 - DlgHeight / 2;

   // The dialog is outside the screen, move it inside
   if( xLeft < rcArea.left ) xLeft = rcArea.left;
   else if( xLeft + DlgWidth > rcArea.right ) xLeft = rcArea.right - DlgWidth;
   if( yTop < rcArea.top ) yTop = rcArea.top;
   else if( yTop + DlgHeight > rcArea.bottom ) yTop = rcArea.bottom - DlgHeight;
   ::SetWindowPos(m_hWnd, NULL, xLeft, yTop, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}


LRESULT HWndObject::SendMessage(UINT uMsg, WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/)
{
   ASSERT(::IsWindow(m_hWnd));
   return ::SendMessage(m_hWnd, uMsg, wParam, lParam);
} 

LRESULT HWndObject::PostMessage(UINT uMsg, WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/)
{
   ASSERT(::IsWindow(m_hWnd));
   return ::PostMessage(m_hWnd, uMsg, wParam, lParam);
}

VOID HWndObject::Resize(int cx /*= -1*/, int cy /*= -1*/)
{
   ASSERT(::IsWindow(m_hWnd));
   RECT rc = { 0 };;
   if( !::GetClientRect(m_hWnd, &rc) ) return;
   if( cx != -1 ) rc.right = cx;
   if( cy != -1 ) rc.bottom = cy;
   if( !::AdjustWindowRectEx(&rc, 
	   GetWindowStyle(m_hWnd), 
	   (!(GetWindowStyle(m_hWnd) & WS_CHILD) && (::GetMenu(m_hWnd) != NULL)), 
	   GetWindowExStyle(m_hWnd)) ) return;

   UINT uFlags = SWP_NOZORDER | SWP_NOMOVE;
   ::SetWindowPos(m_hWnd, NULL, 0, 0, rc.right - rc.left, rc.bottom - rc.top, uFlags);
}


LRESULT HWndObject::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	shared_ptr<XEvent> evt=shared_ptr<XEvent>(new XEvent(m_hWnd,uMsg,wParam,lParam));
	if(WM_PAINT==uMsg)
	{
		//double buffer waper
		evt->BeginDraw();
		DispatchEvent(evt);
		evt->EndDarw();
		return 0;
	}
	else if(WM_ERASEBKGND==uMsg)
	{
		return 0;
	}
	else
	{
		DispatchEvent(evt);
		return !evt->i_bSkip?evt->Value():
			CallWindowProc(m_oldWndProc, m_hWnd, uMsg, wParam, lParam);
	}
}
VOID HWndObject::Move(int x,int y,int cx,int cy,BOOL bRedraw)
{
	MoveWindow(m_hWnd,x,y,cx,cy,TRUE);
}

VOID HWndObject::SetFont( int fIndex,BOOL bRedraw )
{
	HFONT font= UIManager::GetFont(fIndex);
	if(!font)return;
	SetWindowFont(m_hWnd,font,bRedraw);
}

BOOL HWndObject::SetText( std::string text )
{
	if(IsWindow(m_hWnd))return FALSE;
	m_sText=text;
	return SetWindowText(m_hWnd,m_sText.c_str());
}

VOID HWndObject::SetCursor(int hCursor)
{
	::SetCursor(LoadCursor(NULL,MAKEINTRESOURCE(hCursor)));
}

VOID HWndObject::UpdateLayout()
{
	m_bResizeNeeded=TRUE;
	InvalidateRect(m_hWnd, NULL, FALSE);
}

VOID HWndObject::Invalidate( int x,int y,int cx,int cy )
{
	RECT rc;
	SetRect(&rc,x,y,x+cx,y+cy);
	InvalidateRect(m_hWnd,IsRectEmpty(&rc)?NULL:&rc,FALSE);
}


