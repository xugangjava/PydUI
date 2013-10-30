import win32gui
import win32api
from win32con import *
from ui.core.UIManager import XUIManager


XEVT__FIRST = WM_USER + 1
XEVT_MOUSEMOVE = WM_USER + 2
XEVT_MOUSELEAVE = WM_USER + 3
XEVT_MOUSEENTER = WM_USER + 4
XEVT_MOUSEHOVER = WM_USER + 5
XEVT_KEYDOWN = WM_USER + 6
XEVT_KEYUP = WM_USER + 7
XEVT_CHAR = WM_USER + 8
XEVT_SYSKEY = WM_USER + 9
XEVT_KILLFOCUS = WM_USER + 10
XEVT_SETFOCUS = WM_USER + 11
XEVT_BUTTONDOWN = WM_USER + 12
XEVT_BUTTONUP = WM_USER + 13
XEVT_DBLCLICK = WM_USER + 14
XEVT_CONTEXTMENU = WM_USER + 15
XEVT_VSCROLL = WM_USER + 16
XEVT_HSCROLL = WM_USER + 17
XEVT_SCROLLWHEEL = WM_USER + 18
XEVT_WINDOWSIZE = WM_USER + 19
XEVT_SETCURSOR = WM_USER + 20
XEVT_MEASUREITEM = WM_USER + 21
XEVT_DRAWITEM = WM_USER + 22
XEVT_TIMER = WM_USER + 23
XEVT_NOTIFY = WM_USER + 24
XEVT_COMMAND = WM_USER + 25
XEVT__LAS = WM_USER + 26


class XDraw:
    Default = 1
    ThreeInOneH = 2
    ThreeInOneV = 3
    FiveInOneH = 4
    FiveInOneV = 5
    NineInOne = 6


class XFont:
    XFONT_NORMAL = 1
    XFONT_BOLD = 2
    XFONT_CAPTION = 3
    XFONT_MENU = 4
    XFONT_LINK = 5
    XFONT_TITLE = 6
    XFONT_HEADLINE = 7
    XFONT_SUBSCRIPT = 8


def MapKeyState():
    wKeyState = 0
    if win32api.GetKeyState(VK_CONTROL) < 0:
        wKeyState |= MK_CONTROL
    if win32api.GetKeyState(VK_RBUTTON) < 0:
        wKeyState |= MK_LBUTTON
    if win32api.GetKeyState(VK_LBUTTON) < 0:
        wKeyState |= MK_RBUTTON
    if win32api.GetKeyState(VK_SHIFT) < 0:
        wKeyState |= MK_SHIFT
    if win32api.GetKeyState(VK_MENU) < 0:
        wKeyState |= 0x20 # MK_ALT

    return wKeyState


def MAKELONG(l, h):
    return ((h & 0xFFFF) << 16) | (l & 0xFFFF)


MAKELPARAM = MAKELONG


class XEvent:
    def __init__(self, uMsg, evt):
        self.uMsg = uMsg
        if isinstance(evt, int):
            self.spt = self.cpt = 0, 0
            self.wKeyState = MapKeyState()
            self.hWnd = evt
            self.tick = win32api.GetTickCount()
            self.wParam = self.lParam = 0
            self.spt = win32gui.GetCursorPos()
            if evt and win32gui.IsWindow(evt):
                self.cpt = win32gui.ScreenToClient(evt, self.spt)
        else:
            self.cpt = evt.cpt
            self.spt = evt.spt
            self.wKeyState = evt.wKeyState
            self.hWnd = evt.hWnd
            self.tick = evt.tick
            self.lParam = evt.lParam
            self.wParam = evt.wParam

    def __cmp__(self, other):
        if self.uMsg > other:
            return 1
        elif self.uMsg == other:
            return 0
        else:
            return -1

    @staticmethod
    def New(uMsg, evt=NULL):
        return XEvent(uMsg, evt)


class XObject:
    def __init__(self, config):
        self.child = []
        self.layers = []
        self.parent = None
        self.config = {}
        self.config.update(config)
        self.Id = XUIManager.NewId()
        if self.VTuple('xy'):
            self.x, self.y, self.cx, self.cy = self.VTuple('xy')
        else:
            self.x, self.y, self.cx, self.cy = \
                self.VInt('x'), self.VInt('y'), self.VInt('cx'), self.VInt('cy')

        if 'child' in self.config:
            for c in self.config['child']:
                obj = c['xtype'](c)
                obj.parent = self
                self.child.append(obj)

    def GetHWnd(self):
        w = self.GetWnd()
        if w:
            return w.hWnd

    def GetWnd(self):
        p = self.parent
        while p:
            if hasattr(p, 'hWnd'):
                return p
            p = self.parent

    def PointIn(self, x, y):
        return self.x < x < self.x + self.cx and self.y < y < self.cy

    def ReSizer(self):
        """do resizer"""

    def GetXY(self):
        return self.x, self.y, self.cx, self.cy

    def VTuple(self, key, d=()):
        return self.config.get(key, d)

    def VInt(self, key, d=0):
        return self.config.get(key, d)

    def VStr(self, key, d=''):
        return self.config.get(key, d)

    def VBool(self, key, d=False):
        return self.config.get(key, d)

    def VObject(self, key, d=None):
        return self.config.get(key, d)

    def HandlerEvent(self, evt):
        """HandlerEvent"""

    def ProcessEvent(self, evt):
        """ProcessEvent"""

    def Dispose(self):
        """Dispose"""

    def Init(self):
        """do init"""

    def _OnEvent(self, evt):
        if evt == WM_PAINT:
            self.DoPaint(evt)
        else:
            r = self.ProcessEvent(evt)
            if not r:
                return
            self.HandlerEvent(r)

    def DoPaint(self, evt):
        """do paint"""
        for l in self.layers:
            l.Draw(evt.tempDC)


class XControlObject(XObject):
    """control object"""

    def __init__(self, config):
        XObject.__init__(self, config)
        self.bEnable = True
        self.bMouseIn = False
        self.bFouce = False

    def Enable(self, enable=True):
        self.bIsEnable = enable


    def Move(self, x, y):
        self.x, self.y = x, y

    def Resize(self, w, h):
        self.cx, self.cy = w, h

    def ProcessEvent(self, evt):
        """return evt to handler eve return none to skip the event"""
        if evt == WM_MOUSEMOVE:
            if self.PointIn(evt.cpt[0], evt.cpt[1]):
                if not self.bMouseIn:
                    self.bMouseIn = True
                    return XEvent.New(XEVT_MOUSEENTER, evt)
                self.bMouseIn = True
            else:
                if self.bMouseIn:
                    self.bMouseIn = False
                    return XEvent.New(XEVT_MOUSELEAVE, evt)
                self.bMouseIn = False

        if not self.bMouseIn:
            if WM_MOUSEFIRST <= evt <= WM_MOUSELAST:
                return

            if evt in (WM_SETCURSOR,):
                return

        if evt in (WM_SETFOCUS,):
            return

        if evt == XEVT_KILLFOCUS:
            self.bFouce = False

        elif evt == XEVT_SETFOCUS:
            self.bFouce = True

        elif evt == WM_LBUTTONDOWN:
            if not self.bFouce and self.bEnable:
                self.SetFouce()

        return evt

    def SetFouce(self):
        hWnd = self.GetHWnd()
        self.ProcessEvent(XEvent.New(XEVT_SETFOCUS, hWnd))
        obj = XUIManager.GetFouce()
        if obj and obj != self:
            obj.ProcessEvent(XEVT_KILLFOCUS, hWnd)
        XUIManager.SetFouce(self)


