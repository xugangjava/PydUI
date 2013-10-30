#coding=gbk

#===========================================
#作者:许刚
#日期: 13-7-31
#时间: 上午8:32
#To change this template use File | Settings | File Templates.
#===========================================
import ui.core._core
from ui.control.Base import XObject, XDraw
from ui.core.Loader import RCLoader
import cStringIO
XImage = ui.core._core.XImage


class Layer(XImage, XObject):
    def __init__(self, config):
        XObject.__init__(self, config)
        buf = RCLoader.Load(self.VStr("image"))
        XImage.__init__(self, buf)
        self.drawType = self.VInt('drawtype', 1)

    def ReSizer(self):
        if self.VBool('center'):
            self.x += (self.parent.cx - self.cx) / 2
            self.y += (self.parent.cy - self.cy) / 2
            assert self.x > 0
            assert self.y > 0

    def Draw(self, dc):
        if self.drawType == XDraw.Default:
            XImage.DrawDefault(self, dc, self.x, self.y, self.cx, self.cy)
        elif self.drawType == XDraw.ThreeInOneH:
            XImage.DrawDefault(self, dc, self.x, self.y, self.cx, self.cy)
        elif self.drawType == XDraw.ThreeInOneV:
            XImage.DrawDefault(self, dc, self.x, self.y, self.cx, self.cy)
        elif self.drawType == XDraw.FiveInOneH:
            XImage.DrawDefault(self, dc, self.x, self.y, self.cx, self.cy)
        elif self.drawType == XDraw.FiveInOneV:
            XImage.DrawDefault(self, dc, self.x, self.y, self.cx, self.cy)
        elif self.drawType == XDraw.NineInOne:
            XImage.DrawNineInOne(self, dc, self.x, self.y, self.cx, self.cy)