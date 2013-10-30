#coding=gbk
import win32gui
from win32con import *
from ui.control.Base import XObject
import ui.core._core

HWndObject = ui.core._core.HWndObject


class XWindow(XObject, HWndObject):
    def __init__(self, config):
        XObject.__init__(self, config)
        HWndObject.__init__(self, self.__class__.__name__, self.VStr("sbClass"))
        self.dwExStyle = self.VInt('dwExStyle')
        self.dwStyle = self.VInt('dwStyle')
        self.wTitle = self.VStr('wTitle')
        if 'layers' in self.config:
            for c in self.config['layers']:
                obj = c['xtype'](c)
                obj.parent = self

    def Create(self, pHWnd=NULL):
        HWndObject.Create(self,
                          pHWnd,
                          self.wTitle,
                          self.dwStyle,
                          self.dwExStyle,
                          self.x, self.y, self.cx, self.cy,
                          NULL)
        assert self.hWnd
        return self.hWnd

    def DispatchEvent(self, evt):
        statck = [self]
        while statck:
            obj = statck.pop()
            obj._OnEvent(evt)
            if not hasattr(obj, 'child'): continue
            statck.extend(obj.child)

    def ProcessEvent(self, evt):
        if evt == WM_LBUTTONDOWN:
            win32gui.SetFocus(self.hWnd)

    def OnFinalMessage(self):
        """final realse"""

    def Resize(self, cx, cy):
        self.cx, self.cy = cx, cy

    def Move(self, x, y, cx=0, cy=0, reDraw=False):
        if cx:
            self.cx = cx
        if cy:
            self.cy = cy
        HWndObject.Move(self, x, y, self.cx, self.cy, reDraw)


        # def CenterWindow(self):
        # 	HWndObject.CenterWindow(self)
        #
        # def SendMessage(self, uMsg, wParam=0, lParam=0):
        # 	HWndObject.SendMessage(self, uMsg, wParam, lParam)
        #
        # def PostMessage(self, uMsg, wParam=0, lParam=0):
        # 	HWndObject.PostMessage(self, uMsg, wParam, lParam)
        #
        # def Resize(self, cx=-1, cy=-1):
        # 	HWndObject.Resize(self, cx, cy)
        #
        #
        # def SetFont(self,fIndex,bReDraw=TRUE):
        # 	HWndObject.SetFont(fIndex,bReDraw)
        #
        #
        # def Subclass(self, hWnd):
        # 	HWndObject.Subclass(self, hWnd)
        #
        # def Unsubclass(self):
        # 	HWndObject.Unsubclass(self)
        #
        #
        # def ShowWindow(self, bShow=True, bTakeFocus=False):
        # 	HWndObject.ShowWindow(self, bShow, bTakeFocus)
        #
        # def Close(self):
        # 	HWndObject.Close(self)
        #
        # def ShowModal(self):
        # 	HWndObject.ShowModal(self)
        #
        # def SetFont(self,fIndex,bReDraw=True):
        # 	HWndObject.SetFont(self,fIndex,bReDraw)


