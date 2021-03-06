#include "stdafx.h"

#include "UIClass.h"
#include <Psapi.h>
UIClass* UIClassC=0;
DEFINEFUNCTION DragWindowFun(LTExecState* es)
{
	UIClassC->DragWindow();
	return JsUndefined();
}

DEFINEFUNCTION QuitAppFun(LTExecState* es)
{
	UIClassC->QuitApp();
	return JsUndefined();
}
DEFINEFUNCTION MinWindowFun(LTExecState* es)
{
	UIClassC->MinWindow();
	return JsUndefined();
}
DEFINEFUNCTION f1Fun(LTExecState* es)
{
	UIClassC->f1();
	return JsUndefined();
}
DEFINEFUNCTION f2Fun(LTExecState* es)
{
	const wchar_t* str=L"JS 调用C++ 返回了值";
	return String2JsValue(es,str);
}
DEFINEFUNCTION f3Fun(LTExecState* es)
{
	LTEncodedJSValue value = CreateObject(es);
	CreateJsObject(es,value,"avalue",Int2JsValue(1));
	CreateJsObject(es,value,"bvalue",Int2JsValue(2));
	return value;
}
DEFINEFUNCTION f4Fun(LTExecState* es)
{

	LTEncodedJSValue fun = GetJsParameter(es,0);
	wchar_t* str = L"C++执行了回掉函数";
	LTEncodedJSValue arg =String2JsValue(es,str);
	LTEncodedJSValue ptarg[1] = {arg};
	JsCall(es,fun,ptarg,1); 
	return JsUndefined();
}
DEFINEFUNCTION f5Fun(LTExecState* es)
{
	UIClassC->f5(es);
	return JsUndefined();
}

UIClass::UIClass()
{	
	HMODULE hModule = ::GetModuleHandle(0);
	applicationpath  = (wchar_t*)malloc((MAX_PATH+1)*sizeof(wchar_t)); 
	GetModuleFileName(NULL, applicationpath, MAX_PATH);
	(wcsrchr(applicationpath, '\\'))[1] = 0;
	int ww = 700,wh=500;
	OleInitialize(0);


	ltskinview= CreateLTFrameInstance(this,L"LTFrame-javascript",NULL,WS_POPUP | WS_VISIBLE,0,0,ww,wh,0);
	UIClassC =this;
	ltskinview->BindUserFunction("DragWindow",DragWindowFun,0);
	ltskinview->BindUserFunction("QuitApp",QuitAppFun,0);
	ltskinview->BindUserFunction("MinWindow",MinWindowFun,0);
	ltskinview->BindUserFunction("f1",f1Fun,0);
	ltskinview->BindUserFunction("f2",f2Fun,0);
	ltskinview->BindUserFunction("f3",f3Fun,0);
	ltskinview->BindUserFunction("f4",f4Fun,0);
	ltskinview->BindUserFunction("f5",f5Fun,1);

	SetWindowText(ltskinview->windowHandle(),L"LTFrame-javascript");	


	wstring path = wstring(applicationpath)+L"./template/javascript.html";
	ltskinview->loadFile(path.c_str());
	ltskinview->EnableDragFrameChangeSize(false);
	ltskinview->MessageLoop();
}


UIClass::~UIClass(void)
{
	OleUninitialize();
	free(applicationpath);
	delete ltskinview;
}

void UIClass::f1()
{
	ltskinview->RunJavaScript(L"alert('C++执行了JS')");
}
void UIClass::f5(LTExecState* es)
{
	LTEncodedJSValue obj = GetJsParameter(es,0);
	LTExecState exec = ltskinview->GetGlobalExecState();
	LTEncodedJSValue v = GetJsObjectAttribute(exec,obj,"a");
	LTEncodedJSValue xc = GetJsObjectAttribute(exec,obj,"b");
	int i1 = JsValue2Int(exec,v);
	int i2 = JsValue2Int(exec,xc);
	wchar_t * str =new wchar_t[100];
	wsprintf(str,L"alert('a:%d,b:%d')",i1,i2);
	ltskinview->RunJavaScript(str);
	delete[] str;
}
void UIClass::DragWindow()
{
	ReleaseCapture();
	POINT DownPoint;
	GetCursorPos(&DownPoint);
	SendMessage(ltskinview->windowHandle(),WM_NCLBUTTONDOWN,HTCAPTION,MAKEWPARAM(DownPoint.x,DownPoint.y));
}
void UIClass::QuitApp()
{
	ltskinview->QuitApp();
}
void UIClass::MinWindow()
{
	::ShowWindow(ltskinview->windowHandle(),SW_MINIMIZE);	
}
LRESULT UIClass::HandleUserMessage(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{

	return ltskinview->HandleMessage(hwnd,uMsg,wParam,lParam);

}
LPCWSTR UIClass::GetWindowClassName()
{
	return L"javascript";
}
