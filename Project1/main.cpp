#include <Windows.h>
#include <tchar.h>                  
#include <atlstr.h>
#include <windef.h>
#include <iostream>
#include <string>
#include "KeyBoardDevice.h"   
#include "MouseDevice.h"

#define is_empty_LPTSTR(v)			( (v)[0] == _T('\0') )
#define set_LPTSTR_to_empty(v)		( (v)[0] = _T('\0') )

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
// #pragma comment(lib, "msimg32.lib")

using std::cout;
using std::endl;


static KeyboardDevice* g_pKbd = NULL; // 键盘设备
static MouseDevice* g_pMs = NULL; // 键盘设备
static LPDIRECTINPUT8 g_DirectInput = NULL; // directinput接口

static HWND				g_hwnd = NULL;
static HINSTANCE		g_hInst = NULL;
TCHAR str[] = _T("Hello World!");

std::wstring keyboard_str;
std::wstring mouse_str;


void exit_if_failed(HRESULT hr, LPCTSTR szFuncName)
{
	if (FAILED(hr)) {
		CString strMsg;
		strMsg.Format(_T("%s() FAILED!(hr=0x%x)"), szFuncName, hr);

		MessageBox(g_hwnd, strMsg, _T("Error"), 16);

		if (g_DirectInput != NULL) g_DirectInput->Release();
		exit(1);
	}
}



void OnPaint(HWND hwnd) {
	HDC hdc;
	PAINTSTRUCT ps;
	

	hdc = BeginPaint(hwnd, &ps);  // 绘图设备句柄
	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(200,200,200));
	SelectObject(hdc, hPen);
	HBRUSH hBrush = CreateSolidBrush(RGB(200, 200, 200));
	SelectObject(hdc, hBrush);
	Rectangle(hdc, 100, 100, 300, 300);

	// if (!keyboard_str.empty()) {
		SetTextColor(hdc, RGB(0, 0, 0));
		SetBkMode(hdc, TRANSPARENT);
		RECT rc_ms{ 100,100,200,200 };
		DrawText(hdc, mouse_str.data(), _tcslen(mouse_str.data()), &rc_ms, DT_LEFT | DT_TOP);
		RECT rc_kbd{ 100,150,200,200 };
		DrawText(hdc, keyboard_str.data(), _tcslen(keyboard_str.data()), &rc_kbd, DT_LEFT | DT_TOP);
	// }

	DeleteDC(hdc);
	DeleteObject(hPen);
	DeleteObject(hBrush);


	EndPaint(hwnd, &ps);
}
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg)
	{
	case WM_PAINT:
		OnPaint(hwnd);
		break;
	case WM_CREATE:
		// 将窗口设置为顶层
		SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE| SWP_NOSIZE);
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


void processMouseInput() {
	if (!g_pMs->read()) {
		cout << "READ MOUSE FAILED" << endl;
		return;
	}

	DIMOUSESTATE* buffer = g_pMs->getState();
	cout << buffer->lX << endl;

	LPTSTR delta_xyz = new TCHAR[1024];
	wsprintf(delta_xyz, L"%3ld, %3ld, %3ld", buffer->lX, buffer->lY, buffer->lZ);
	mouse_str = std::wstring(delta_xyz);

	std::wcout << mouse_str << endl;
}

void processKeyboardInput() {
	if (!g_pKbd->read()) {
		cout << "READ KEYBOARD FAILED" << endl;
		return;
	}
	
	BYTE* buffer = g_pKbd->getState();
	CString result;
	int key_count = 0;

	for (UINT i = 0; i <= 0xff; ++i) {
		BYTE state = buffer[i];
		if (state & 0x80) {
			LPCTSTR curr_key_name = g_pKbd->translate(i);
			if (!is_empty_LPTSTR(curr_key_name)) {
				if (!result.IsEmpty()) {
					result += _T(", ");
				}
				result += curr_key_name;
				key_count++;
			}
		}
	}
	
	keyboard_str = std::wstring(result.GetBuffer());

	std::wcout << keyboard_str << endl;
	
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpCmdLine, int nCmdShow) {
	// MessageBox(NULL, L"Hello", L"First Project", MB_OK);
	wchar_t szClassName[] = L"Demo";

	// For Console Debug
	AllocConsole();
	FILE* pOut = NULL;
	freopen_s(&pOut, "conout$", "w", stdout);


	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;// 窗口类的风格
	wc.lpfnWndProc = WindowProc;       // 窗口处理函数
	wc.cbClsExtra = 0;                 // 窗口类额外空间内存大小
	wc.cbWndExtra = 0;                 // 窗口额外控件内存大小
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = CreateSolidBrush(RGB(200,200,200));
	wc.lpszMenuName = NULL;
	wc.lpszClassName = szClassName;

	// 注册窗口
	if (FALSE == RegisterClass(&wc)) {
		MessageBox(NULL, L"Failed", L"Fuck", MB_OK);
	}

	// 创建窗口
	g_hwnd = CreateWindow(
		szClassName,
		L"Title",
		WS_BORDER | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX,
		200, // x
		200, // y 
		800, // width
		600, // height
		NULL, // parent
		NULL, // menu
		hInstance, // self
		NULL // 附加参数
	);

	// 显示窗口
	ShowWindow(g_hwnd, SW_SHOW);
	// 更新窗口
	UpdateWindow(g_hwnd);
	InvalidateRect(g_hwnd, NULL, TRUE);

	HRESULT hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, reinterpret_cast<void**>(&g_DirectInput), NULL);
	if (!g_hwnd) {
		MessageBox(NULL, _T("Cannot create DirectInput8 device"), _T("Error"), 16);
		return 126;
	}
	g_pKbd = new KeyboardDevice(g_hwnd, g_DirectInput);
	g_pMs = new MouseDevice(g_hwnd, g_DirectInput);
	KeyboardDevice::lasterror_t le_kbd = g_pKbd->getLastError();
	exit_if_failed(le_kbd.hr, le_kbd.func_name);

	MouseDevice::lasterror_t le_ms = g_pMs->getLastError();
	exit_if_failed(le_ms.hr, le_ms.func_name);

	MSG msg = { 0 };
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) { // 处理窗口相关的消息
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			//cout << "HHH" << endl;
			processKeyboardInput();
			processMouseInput();
			InvalidateRect(g_hwnd, NULL, FALSE);
			Sleep(7);
		}
	}



	return 0;
}

