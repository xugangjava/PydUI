#include "stdafx.h"
#include "XUIManager.h"
#include "XHWndObject.h"
#include "XLock.h"
#include "XEvent.h"
#include "XImage.h"

using namespace boost;
using namespace boost::python;
using namespace std;



class HWndObjectWapper:public HWndObject,public wrapper<HWndObject>
{
public:
	HWndObjectWapper(){}
	HWndObjectWapper(string className,string sbClassName)
		:HWndObject(className,sbClassName){}
	~HWndObjectWapper(){}


	VOID DispatchEvent(shared_ptr<XEvent> evt)
	{
		XLock lock;
		this->get_override("DispatchEvent")(evt);
	};


	VOID OnFinalMessage()
	{
		XLock lock;
		this->get_override("OnFinalMessage")();
	}
};


BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(InvalidateOVL, Invalidate,0, 4)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(ShowWindowOVL, ShowWindow,0, 2)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(SendMessageOVL, SendMessage,1, 3)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(PostMessageOVL, PostMessage,1, 3)

BOOST_PYTHON_MODULE(_core)
{
	class_<XEvent ,noncopyable,shared_ptr<XEvent> >("XEvent",no_init)
		.def("Skip",&XEvent::Skip)
		.def("Stop",&XEvent::Stop)
		.def("Default",&XEvent::Default)
		.def("__cmp__",&XEvent::Cmp)
		.add_property("wKeyState",&XEvent::GetWKeyState)
		.add_property("cpt",&XEvent::GetClientPos)
		.add_property("spt",&XEvent::GetScreenPos)
		.add_property("hWnd",&XEvent::GetHandler)
		.add_property("tick",&XEvent::GetTime)
		.add_property("lParam",&XEvent::GetLParam)
		.add_property("wParam",&XEvent::GetWParam)
		.add_property("dc",&XEvent::GetDC)
		.add_property("tempDC",&XEvent::GetTempDC)
	;

	class_<UIManager,noncopyable>("UIManager",no_init)
		.def("Init",&UIManager::Init).staticmethod("Init")
		.def("Dispose",&UIManager::Dispose).staticmethod("Dispose")
		.def("TranslateMessage",&UIManager::TranslateMessage).staticmethod("TranslateMessage")
		.def("NewId",&UIManager::NewId).staticmethod("NewId")
		.def("MessageLoop",&UIManager::MessageLoop).staticmethod("MessageLoop")
	;

	
	class_<HWndObjectWapper,shared_ptr<HWndObjectWapper>, noncopyable>("HWndObject")
		.def(init<std::string,std::string>())
		.def("Create", &HWndObjectWapper::Create)  
		.def("SetCursor",&HWndObjectWapper::SetCursor)
		.def("Invalidate",&HWndObjectWapper::Invalidate,InvalidateOVL())
		.def("UpdateLayout",&HWndObjectWapper::UpdateLayout)
		.def("SetFont",&HWndObject::SetFont)
		.def("SetText",&HWndObject::SetText)
		.def("GetText",&HWndObject::GetText)
		.def("Move",&HWndObjectWapper::Move)
		.def("CenterWindow",&HWndObjectWapper::CenterWindow)
		.def("SendMessage",&HWndObjectWapper::SendMessage,SendMessageOVL())
		.def("PostMessage",&HWndObjectWapper::PostMessage,PostMessageOVL())
		.def("Resize",&HWndObjectWapper::Resize)
		.def("Subclass",&HWndObjectWapper::Subclass)
		.def("Unsubclass",&HWndObjectWapper::Unsubclass)
		.def("ShowWindow",&HWndObject::ShowWindow,ShowWindowOVL())
		.def("Close",&HWndObjectWapper::Close)
		.def("ShowModal",&HWndObjectWapper::ShowModal)
		.add_property("hWnd",&HWndObjectWapper::GetHandler)
		//virtural
		.def("DispatchEvent", pure_virtual(&HWndObject::DispatchEvent))
		.def("OnFinalMessage", pure_virtual(&HWndObject::OnFinalMessage))
	;

	class_<XImage ,noncopyable,shared_ptr<XImage>,noncopyable>("XImage")
		.def(init<std::string>())
		.def("Draw",&XImage::Draw)
		.def("DrawDefault",&XImage::DrawDefault)
		.def("DrawNineInOne",&XImage::DrawNineInOne)
		.def("DrawThreeInOneH",&XImage::DrawThreeInOneH)
		.def("DrawThreeInOneV",&XImage::DrawThreeInOneV)
		.def("DrawFiveInOneH",&XImage::DrawFiveInOneH)
		.def("DrawFiveInOneV",&XImage::DrawFiveInOneV)
	
	;

}
