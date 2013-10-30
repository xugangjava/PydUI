#coding=gbk

#===========================================
#作者:许刚
#日期: 13-7-31
#时间: 上午9:15
#caption control
#===========================================
from ui.control.Base import XObject
from win32con import *
import win32gui


class XCaption(XObject):
    def __init__(self, config):
        XObject.__init__(self, config)
        self.bDraging = False
        self.startX = 0
        self.startY = 0
        self.winX = 0
        self.winY = 0

    def ProcessEvent(self, evt):
        x, y = evt.cpt
        sx, sy = evt.spt
        if self.PointIn(x, y) and evt == WM_LBUTTONDOWN and not self.bDraging:
            self.startX, self.startY = sx, sy
            self.winX, self.winY, buttom, rignt = win32gui.GetWindowRect(evt.hWnd)
            win32gui.SetCapture(evt.hWnd)
            self.bDraging = True

        if self.bDraging and evt == WM_LBUTTONUP:
            win32gui.ReleaseCapture()
            self.bDraging = False

        if self.bDraging and evt == WM_MOUSEMOVE:
            win = self.GetWnd()
            win.Move(self.winX + (sx - self.startX), self.winY + (sy - self.startY))
