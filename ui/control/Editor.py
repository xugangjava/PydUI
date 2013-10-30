#coding=gbk
#===========================================
#作者:许刚
#日期: 13-7-24
#时间: 上午9:08
#To change this template use File | Settings | File Templates.
#===========================================
import ui.core._core
from win32con import *
import win32gui
from ui.control.Base import XControlObject, \
    XEVT_SETFOCUS, XEVT_KILLFOCUS, \
    XFont, MAKELPARAM
HWndObject = ui.core._core.HWndObject


class XSingleLineEditor(XControlObject, HWndObject):
    def __init__(self, config):
        XControlObject.__init__(self, config)
        HWndObject.__init__(self, self.__class__.__name__, 'Edit')
        self.dwExStyle = 0
        self.dwStyle = ES_AUTOHSCROLL | WS_CHILD
        self.wTitle = ''
        self.sText = ''
        self.bReadOnly = False

    def GetReft(self):
        return -1, -3

    def Dispose(self):
        """do dispose"""
        self.pWindow = None
        print self.hWnd

    def Init(self):
        pHWnd = self.GetHWnd()
        dx, dy = self.GetReft()
        self.pWindow = HWndObject.Create(self,
                                         pHWnd,
                                         self.wTitle,
                                         self.dwStyle,
                                         self.dwExStyle,
                                         self.x, self.y, self.cx + dx, self.cy + dy,
                                         NULL)
        assert self.hWnd
        self.SetFont(XFont.XFONT_NORMAL, TRUE)
        self.SetText(self.sText)
        self.SendMessage(EM_SETMODIFY, FALSE, 0)
        self.SendMessage(EM_SETMARGINS,
                         EC_LEFTMARGIN | EC_RIGHTMARGIN, MAKELPARAM(2, 2))
        self.SendMessage(EM_SETSEL, 0, -1)
        win32gui.EnableWindow(self.pWindow, self.bEnable)
        self.SetReadOnly(self.bReadOnly)
        win32gui.ShowWindow(self.pWindow, SW_SHOWNOACTIVATE)
        win32gui.SetFocus(self.pWindow)

    def SetReadOnly(self, bReadOnly):
        self.bReadOnly = bReadOnly
        self.SendMessage(EM_SETREADONLY, bReadOnly, 0)

    def OnFinalMessage(self):
        self.Dispose()

    def DispatchEvent(self, evt):
        """DispatchEvent"""
        if evt == WM_KILLFOCUS:
            r = evt.Default()
            print win32gui.GetClassName(self.hWnd) + "close"
            self.PostMessage(WM_CLOSE)
            evt.Stop(r)

            #todo set fouce

    def HandlerEvent(self, evt):
        if evt == WM_SETCURSOR:
            self.SetCursor(IDC_IBEAM)
            evt.Stop(TRUE)
        elif evt == WM_SIZE:
            pass

        elif evt == XEVT_SETFOCUS:
            if not self.bEnable: return

        elif evt == XEVT_KILLFOCUS:
            pass

        elif evt == WM_LBUTTONDOWN:
            self.Init()






