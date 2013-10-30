#coding=gbk

import win32gui
from win32con import *
from ui.control.Base import XDraw
from ui.control.Caption import XCaption
from ui.core.UIManager import XUIManager
from ui.control.Window import XWindow
from ui.control.Editor import XSingleLineEditor
from ui.darw.Canvas import Layer

if __name__ == '__main__':
    XUIManager.Init()
    w = XWindow({
        'cx': 435,
        'cy': 302,
        'dwStyle': WS_POPUP,
        'classStyle': CS_HREDRAW | CS_VREDRAW,
        'layers': [
            {
                'xtype': Layer,
                'center': True,
                'cx': 380,
                'cy': 292,
                'drawtype': XDraw.Default,
                'image': 'login.noon.jpg',
            },
            # {
            # 	'xtype':Mask,
            # 	'relative':True,
            # 	'cx':388,
            # 	'cy':300,
            # 	'center':True,
            # 	'rgba':True,
            # 	'image':'login.projection.png',
            # 	'drawtype':DrawType.ThreeInOneH,
            # 	'drawx':(10,17),
            # },
            # {
            # 	'xtype':Texture,
            # 	'relative':True,
            # 	'cx':390,
            # 	'cy':302,
            #         'gray':True,
            # 	'center':True,
            # 	'image':'qq.window_Shadow_bkg.png.png',
            # 	'drawtype':DrawType.NineInOne,
            # 	'drawx':(14,31),
            # 	'drawy':(15,274)
            # },
            # {
            # 	'xtype':Texture,
            # 	'relative':True,
            # 	'cx':388,
            # 	'cy':300,
            # 	'center':True,
            # 	'image':'login.stroke.png',
            # 	'drawtype':DrawType.ThreeInOneH,
            # 	'drawx':(10,17)
            # },
        ],
        'child': [
            {
                'xtype': XCaption,
                'center': True,
                'cx': 380,
                'cy': 292
            },
        ]
    })
    w.Create()
    w.ShowWindow()
    w.CenterWindow()
    # w.ShowModal()
    XUIManager.MessageLoop()
    XUIManager.Dispose()