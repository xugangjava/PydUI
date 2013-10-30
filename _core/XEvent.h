#pragma once
#include "stdafx.h"

class XEvent
{
public:
	WPARAM      i_wParam;
	LPARAM      i_lParam;
	BOOL i_bSkip;

private:

	LRESULT m_lpResult;

	int        m_hWnd;
	UINT        m_uMsg;

	DWORD       m_time;
	POINT       m_spt;
	POINT m_cpt;
	UINT m_wKeyState;
	
	LPPAINTSTRUCT m_ps;
	HDC m_tempDC;

	HBITMAP m_tempBmp;
	RECT* m_rcWin;

public:
	

	XEvent(void)
	{
		XEvent(NULL,NULL,NULL,NULL);
	}

	
	XEvent::~XEvent(void)
	{

	}

	XEvent(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam):
		m_hWnd((int)hWnd),
		m_uMsg(uMsg),
		i_wParam(wParam),
		i_lParam(lParam),
		i_bSkip(TRUE),
		m_lpResult(NULL),
		m_wKeyState(0)
	{
		GetCursorPos(&m_spt);
		if(hWnd&&IsWindow(hWnd)){
			m_cpt=m_spt;
			ScreenToClient(hWnd,&m_cpt);
		}else{
			m_cpt.x=m_cpt.y=0;
		}
	    	if( ::GetKeyState(VK_CONTROL) < 0 ) m_wKeyState |= MK_CONTROL;
		if( ::GetKeyState(VK_RBUTTON) < 0 ) m_wKeyState |= MK_LBUTTON;
		if( ::GetKeyState(VK_LBUTTON) < 0 ) m_wKeyState |= MK_RBUTTON;
		if( ::GetKeyState(VK_SHIFT) < 0 ) m_wKeyState |= MK_SHIFT;
		if( ::GetKeyState(VK_MENU) < 0 ) m_wKeyState |= MK_ALT;
		m_ps=NULL;
	}


	DWORD GetTime() const { return m_time; }
	UINT GetMsg() const { return m_uMsg; }
	WPARAM GetWParam() const { return i_wParam; }
	LPARAM GetLParam() const { return i_lParam; }
	int GetHandler() const{return (int)m_hWnd;}
	boost::python::tuple GetClientPos() const {return boost::python::make_tuple(m_cpt.x,m_cpt.y);}
	boost::python::tuple GetScreenPos() const {return boost::python::make_tuple(m_spt.x,m_spt.y);}
	UINT GetWKeyState() const { return m_wKeyState; }

	VOID BeginDraw()
	{
		m_ps=new PAINTSTRUCT();
		m_rcWin=new RECT();
		GetWindowRect((HWND)m_hWnd,m_rcWin);
		BeginPaint((HWND)m_hWnd,m_ps);
		int w=m_rcWin->right-m_rcWin->top;
		int h=m_rcWin->bottom-m_rcWin->top;
		m_tempDC=CreateCompatibleDC(m_ps->hdc);
		m_tempBmp=CreateCompatibleBitmap(m_ps->hdc,w,h);
		SelectObject(m_tempDC,m_tempBmp);
	}

	VOID EndDarw()
	{
		int w=m_rcWin->right-m_rcWin->top;
		int h=m_rcWin->bottom-m_rcWin->top;
		BitBlt(m_ps->hdc,0,0,w,h,m_tempDC,0,0, SRCCOPY);
		DeleteObject(m_tempBmp);
		DeleteObject(m_tempDC);
		EndPaint((HWND)m_hWnd,m_ps);
		delete m_rcWin;
		delete m_ps;
		m_ps=NULL;
	}

	int GetTempDC() const{return NULL==m_ps?0:(int)m_tempDC;}
	int GetDC() const {return NULL==m_ps?0:(int)m_ps->hdc;}

	int Cmp(int o)
	{
		if( (int)m_uMsg>o)return 1;
		else if((int)m_uMsg==o)return 0;
		else return -1;
	}

	int Value(){return m_lpResult;}

	VOID Skip(){i_bSkip=TRUE;}

	VOID Stop(int rResult=NULL)
	{
		i_bSkip=FALSE;
		m_lpResult=rResult;
	}

	LRESULT Default()
	{
		return DefWindowProc((HWND)m_hWnd,m_uMsg,i_wParam,i_lParam);
	}

};
