#pragma once
#include "stdafx.h"
#include "XEvent.h"
using namespace boost;

class HWndObject
{
protected:
	HWND m_hWnd;
	WNDPROC m_oldWndProc;
	BOOL m_bSubclassed;
	BOOL m_bResizeNeeded;
	std::string m_sClassName;
	std::string m_sSubClassName;//LPCTSTR
	std::string m_sText;

	UINT m_iClassStyle;
	
//inits
public:

	BOOL bNew;

	HWndObject()
	{

	}

	HWndObject(std::string className,std::string sbClassName)
	{
			m_hWnd=NULL;
			m_oldWndProc=::DefWindowProc;
			m_bSubclassed=FALSE;
			m_sClassName=className;
			m_sSubClassName=sbClassName;
			m_iClassStyle=0;
			bNew=FALSE;
	}


	  virtual ~HWndObject() 
	  {

	  }

public:

	VOID SetFont(int fIndex,BOOL bRedraw);

	BOOL SetText(std::string text);

	std::string GetText() const{return m_sText;}
	
    int Create(int hwndParent, LPCTSTR pstrName, int dwStyle, int dwExStyle, 
		int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int cx = CW_USEDEFAULT, int cy = CW_USEDEFAULT, 
		int hMenu = NULL);

	LRESULT SendMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0L);

    LRESULT PostMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0L);

	VOID Resize(int cx = -1, int cy = -1);

	int Subclass(HWND hWnd);

	VOID Unsubclass();

	VOID ShowWindow(bool bShow = true, bool bTakeFocus = true);

	BOOL ShowModal();

	VOID Close();

	VOID CenterWindow();
	
	int GetHandler(){return (int)m_hWnd;};

	VOID Move(int x,int y,int cx,int cy,BOOL bRedraw);

	VOID SetCursor(int hCursor);

	VOID UpdateLayout();

	VOID Invalidate(int x=0,int y=0,int cx=0,int cy=0);

//virtual
public:
	virtual VOID OnFinalMessage()=0;

	virtual VOID DispatchEvent(shared_ptr<XEvent> evt)=0;



protected:

	static LRESULT CALLBACK __WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    static LRESULT CALLBACK __ControlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	BOOL RegisterWindowClass();
	
	BOOL RegisterSuperclass();

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

};
