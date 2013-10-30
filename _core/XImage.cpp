#include "stdafx.h"
#include "XImage.h"



void XImage::DrawDefault( int dc,int x,int y, int cx,int cy )
{
	if(bmpW!=cx||bmpH!=cy)
	{
		m_Image.BitBlt((HDC)dc,x,y,cx,cy,bmpW,bmpH,SRCCOPY);
	}
	else
	{
		m_Image.StretchBlt((HDC)dc,x,y,cx,cy,0,0,bmpW,bmpH,SRCCOPY);
	}
}

void XImage::DrawNineInOne( int dc, int x,int y, int cx,int cy, int left,int right,int top,int bottom )
{
	HDC tempDC=(HDC)dc;

	int curH=top;
	int yOffset=0;
	int widhtUp=cx-left-(bmpW-right);
	int heightUp=cy-top-(bmpH-bottom);
	HDC srcDC=m_Image.GetDC();
	//1
	StretchBlt (tempDC, x, yOffset, left, curH, 
		srcDC, x, yOffset, left, curH, SRCCOPY );
	//2
	StretchBlt(tempDC, left, yOffset, widhtUp, curH,
		srcDC,left, yOffset, right-left, curH, SRCCOPY);//宽拉伸
	//3
	StretchBlt(tempDC, left+widhtUp, yOffset, bmpW-right, curH,
		srcDC,right, yOffset, bmpW-right, curH, SRCCOPY);
	curH=bottom-top;
	yOffset=top;
	//4
	StretchBlt(tempDC, x, yOffset, left, heightUp,
		srcDC, x, yOffset, left, curH,  SRCCOPY);//长拉伸
	//5
	StretchBlt(tempDC, left, yOffset,widhtUp, curH,
		srcDC,left, yOffset, right-left, curH,SRCCOPY);//宽拉伸
	//6
	StretchBlt(tempDC, left+widhtUp, yOffset, bmpW-right, heightUp,
		srcDC,right, yOffset, bmpW-right, curH, SRCCOPY);//长拉伸

	curH=bmpH-bottom;
	yOffset=bottom;

	//7
	StretchBlt(tempDC, x, top+heightUp, left, curH, 
		srcDC, x, yOffset, left, curH,  SRCCOPY);

	//8
	StretchBlt(tempDC, left, top+heightUp, widhtUp, curH, 
		srcDC,left, yOffset, right-left, curH,SRCCOPY);//宽拉伸

	//9
	StretchBlt(tempDC, left+widhtUp, top+heightUp, bmpW-right, curH,
		srcDC,right, yOffset, bmpW-right, curH, SRCCOPY);

	m_Image.ReleaseDC();
}

void XImage::DrawThreeInOneH( int dc, int x,int y, int cx,int cy, int left,int right )
{
	HDC tempDC=(HDC)dc;
	HDC srcDC=m_Image.GetDC();
	int bmpW=m_Image.GetWidth();
	int bmpH=m_Image.GetHeight();
	int widhtUp=cx-left-(bmpW-right);
	//1
	StretchBlt (tempDC, x, y,left, cy,
		srcDC, 0, 0, left, cy, SRCCOPY );
	//2
	StretchBlt (tempDC,x+left, y,widhtUp, cy,
		srcDC, left, 0, right-left, cy, SRCCOPY );
	//3
	StretchBlt (tempDC,x+left+widhtUp,y,bmpW-right, cy, 
		srcDC, right, 0, bmpW-right, cy, SRCCOPY );
	m_Image.ReleaseDC();
}

void XImage::DrawFiveInOneH( int dc, int x,int y, int cx,int cy, int left,int midLeft,int midRight,int right, int leftUpWidth,int rightUpWidth )
{
	HDC tempDC=(HDC)dc;
	HDC srcDC=m_Image.GetDC();
	int bmpW=m_Image.GetWidth();
	int midUpWidth=cx-leftUpWidth-rightUpWidth-(midLeft-left)-(right-midRight);
	int xPos=x;
	int curWidth=leftUpWidth;

	//1 拉伸
	StretchBlt (tempDC, xPos, y,curWidth, cy,
		srcDC, 0, 0, left, cy, SRCCOPY );

	xPos+=curWidth;
	curWidth=midLeft-left;
	//2
	StretchBlt (tempDC,x+xPos, y,curWidth, cy,
		srcDC, left, 0, curWidth, cy, SRCCOPY );
	xPos+=curWidth;
	curWidth=midUpWidth;

	//3 拉伸
	StretchBlt (tempDC,xPos,y,curWidth, cy, 
		srcDC, midLeft, 0,midRight-midLeft, cy, SRCCOPY );

	xPos+=curWidth;
	curWidth=right-midRight;
	//4
	StretchBlt (tempDC,xPos,y,curWidth, cy, 
		srcDC,midRight, 0, curWidth, cy, SRCCOPY );
	xPos+=curWidth;
	curWidth=rightUpWidth;

	//5 拉伸
	StretchBlt (tempDC,xPos,y,curWidth, cy, 
		srcDC, right, 0, bmpW-right, cy, SRCCOPY );

	m_Image.ReleaseDC();
}

void XImage::DrawFiveInOneV()
{

}

void XImage::DrawThreeInOneV()
{

}

void XImage::Draw()
{

}
