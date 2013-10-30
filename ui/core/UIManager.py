#coding=gbk
import ui.core._core

UIManager = ui.core._core.UIManager


class XUIManager(UIManager):
    __FouceObject = None

    @staticmethod
    def GetFont(fIndex):
        return UIManager.GetFont(fIndex)

    @staticmethod
    def GetFouce():
        return XUIManager.__FouceObject

    @staticmethod
    def SetFouce(obj):
        XUIManager.__FouceObject = obj

    @staticmethod
    def MessageLoop():
        UIManager.MessageLoop()

    @staticmethod
    def Init():
        return UIManager.Init()

    @staticmethod
    def Dispose():
        return UIManager.Dispose()

    @staticmethod
    def TranslateMessage():
        return UIManager.TranslateMessage()

    @staticmethod
    def NewId():
        return UIManager.NewId()

    @staticmethod
    def GetFont(fIndex):
        return UIManager.GetFont(fIndex)

    #todo set font

