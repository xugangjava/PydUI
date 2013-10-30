#pragma once
#include "stdafx.h"
#include <atlimage.h>
namespace bp = boost::python;

class XImage
{
private:
	std::string m_sBuf;
	CImage m_Image;

	int m_DrawType;

	int prevX;
	int prevY;
	int prevCX;
	int prevCY;
	int bmpW;
	int bmpH;

public:

	XImage()
	{
		bmpW=0;
		bmpH=0;
	}

	XImage(bp::dict kv)
	{
		/*int len=bp::len(bufer);
		BYTE* heap=new BYTE[len];
		for(int i = 0; i < len; i++)
			heap[i] = bp::extract< unsigned char >(bufer[i] );

		HGLOBAL hGlobal = ::GlobalAlloc(GHND, len);
		LPBYTE  lpByte  = (LPBYTE)::GlobalLock(hGlobal);
		CopyMemory(lpByte, heap, len);
		::GlobalUnlock(hGlobal);
		IStream* pStream = NULL;
		if ( SUCCEEDED(::CreateStreamOnHGlobal(hGlobal, FALSE, &pStream)) )
		{
			m_Image.Destroy();
			m_Image.Load(pStream);
			pStream->Release();
		}
		GlobalFree(hGlobal);
		delete[] heap;
*/


		bmpW=m_Image.GetWidth();
		bmpH=m_Image.GetHeight();

	}

	~XImage(void)
	{
		m_Image.Destroy();
	}

public:

	//bool Create(BYTE* heap, DWORD heap_len, CImage& img)
	//{
	//	bool ret = false;
	//	HGLOBAL hGlobal = ::GlobalAlloc(GHND, heap_len);
	//	LPBYTE  lpByte  = (LPBYTE)::GlobalLock(hGlobal);
	//	CopyMemory(lpByte, heap, heap_len);
	//	::GlobalUnlock(hGlobal);
	//	IStream* pStream = NULL;
	//	if ( SUCCEEDED(::CreateStreamOnHGlobal(hGlobal, FALSE, &pStream)) )
	//	{
	//		img.Destroy();
	//		img.Load(pStream);
	//		pStream->Release();
	//		ret = true;
	//	}
	//	GlobalFree(hGlobal);
	//	return ret;
	//}

	//draw  
	void Draw();

	void DrawDefault(int dc,int x,int y, int cx,int cy);

	void DrawNineInOne(int dc,
		int x,int y, int cx,int cy,
		int left,int right,int top,int bottom);

	void DrawThreeInOneH(int dc,
		int x,int y, int cx,int cy,
		int left,int right);

	void DrawThreeInOneV();

	void DrawFiveInOneH(int dc,
				int x,int y, int cx,int cy,
				int left,int midLeft,int midRight,int right,
				int leftUpWidth,int rightUpWidth);

	void DrawFiveInOneV();

};

