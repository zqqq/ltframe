#include "stdafx.h"

#include "SubWindow2.h"


namespace SubWindowSpace{
SubWindow2* SubWindowC=0;

DEFINEFUNCTION DragWindowFun2(LTExecState* es)
{
	SubWindowC->DragWindow();
	return JsUndefined();
}


SubWindow2::SubWindow2(HWND hwnd,int x,int y)
{	
	HMODULE hModule = ::GetModuleHandle(0);
	applicationpath  = (wchar_t*)malloc((MAX_PATH+1)*sizeof(wchar_t)); 
	GetModuleFileName(NULL, applicationpath, MAX_PATH);
	(wcsrchr(applicationpath, '\\'))[1] = 0;
	int ww = 700,wh=500;
	OleInitialize(0);
	ltskinview= CreateLTFrameInstance(this,L"LTFrame",hwnd,WS_POPUP | WS_VISIBLE,x,y,ww,wh,0);
	SubWindowC =this;
	ltskinview->BindUserFunction("DragWindow",DragWindowFun2,0);
	//ltskinview->BindUserFunction("CloseWindow",CloseWindowFun,0);
	//ltskinview->BindUserFunction("MinWindow",MinWindowFun,0);
	SetWindowText(ltskinview->windowHandle(),L"LTFrame");
	wstring path = wstring(applicationpath)+L"./template/SubWindow.html";
	ltskinview->loadFile(path.c_str());
	ltskinview->EnableDragFrameChangeSize(false);
	SetForegroundWindow(ltskinview->windowHandle());
	//ltskinview->MessageLoop();
}


SubWindow2::~SubWindow2(void)
{
	free(applicationpath);
}
void SubWindow2::DragWindow()
{
	ReleaseCapture();
	POINT DownPoint;
	GetCursorPos(&DownPoint);
	SendMessage(ltskinview->windowHandle(),WM_NCLBUTTONDOWN,HTCAPTION,MAKEWPARAM(DownPoint.x,DownPoint.y));
}
void SubWindow2::CloseWindow()
{
	DestroyWindow(ltskinview->windowHandle());
}
void SubWindow2::MinWindow()
{
	::ShowWindow(ltskinview->windowHandle(),SW_MINIMIZE);	
}
LRESULT SubWindow2::HandleMessage(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	switch(uMsg)
	{

	}
	return ltskinview->HandleMessage(hwnd,uMsg,wParam,lParam);
}


LPCWSTR SubWindow2::GetWindowClassName()
{
	return L"INDEX";
}
}