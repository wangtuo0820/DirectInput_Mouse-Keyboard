#include "KeyboardDevice.h"


KeyboardDevice::KeyboardDevice(HWND hWnd, LPDIRECTINPUT8 pDirectInput) : m_lasterror(), m_isLastReadingFailed(false) {
	HRESULT hr;
	m_hWnd = hWnd;
	ZeroMemory(m_buffer, sizeof(m_buffer));
	hr = pDirectInput->CreateDevice(GUID_SysKeyboard, &m_kbd, NULL);
	if (!setError(hr, _T("CreateDevice"))) {
		hr = m_kbd->SetDataFormat(&c_dfDIKeyboard);
		if (!setError(hr, _T("SetDataFormat"))) {
			hr = m_kbd->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
			setError(hr, _T("SetCooperativeLevel"));
		}
	}
	init_map();
}

bool KeyboardDevice::dev_read() {
	HRESULT hr;

	m_kbd->Poll();
	m_kbd->Acquire();

	if (SUCCEEDED(hr = m_kbd->GetDeviceState(sizeof(m_buffer), m_buffer)))
		return true;
	if (hr != DIERR_INPUTLOST && hr != DIERR_NOTACQUIRED) return false;
	if (FAILED(m_kbd->Acquire())) return false;

	return false;
}

bool KeyboardDevice::read() {
	Sleep(7);
	if (dev_read()) {
		m_isLastReadingFailed = false;
	}
	else {
		m_isLastReadingFailed = true;
	}

	return !m_isLastReadingFailed;
}

KeyboardDevice::~KeyboardDevice() {
	
	if ((m_kbd) != NULL) { (m_kbd)->Release(); (m_kbd) = NULL; }
}



//// private:

// return true if hr is failed
bool KeyboardDevice::setError(HRESULT hr, LPCTSTR func_name) {
	if (FAILED(hr)) {
		m_lasterror.set(hr, func_name);
		return true;
	}
	else
		return false;
}



void KeyboardDevice::init_map() {
	if (s_map.size() > 0)   // 若已经初始化过，则跳过
		return;

	s_map[0x01] = _T("ESC");
	s_map[0x02] = _T("VK1");          // 主键盘数字键“1”
	s_map[0x03] = _T("VK2");
	s_map[0x04] = _T("VK3");
	s_map[0x05] = _T("VK4");
	s_map[0x06] = _T("VK5");
	s_map[0x07] = _T("VK6");
	s_map[0x08] = _T("VK7");
	s_map[0x09] = _T("VK8");
	s_map[0x0A] = _T("VK9");
	s_map[0x0B] = _T("VK0");
	s_map[0x0C] = _T("MINUS");        // 主键盘区的减号键
	s_map[0x0D] = _T("EQUAL");        // 等号
	s_map[0x0E] = _T("BACK");         // 退格键 backspace
	s_map[0x0F] = _T("TAB");
	s_map[0x10] = _T("Q");
	s_map[0x11] = _T("W");
	s_map[0x12] = _T("E");
	s_map[0x13] = _T("R");
	s_map[0x14] = _T("T");
	s_map[0x15] = _T("Y");
	s_map[0x16] = _T("U");
	s_map[0x17] = _T("I");
	s_map[0x18] = _T("O");
	s_map[0x19] = _T("P");
	s_map[0x1A] = _T("LBRACKET");     // 左方括号 [
	s_map[0x1B] = _T("RBRACKET");     // 右方括号 ]
	s_map[0x1C] = _T("ENTER");        // Enter on main keyboard
	s_map[0x1D] = _T("LCONTROL");     // 左 CTRL
	s_map[0x1E] = _T("A");
	s_map[0x1F] = _T("S");
	s_map[0x20] = _T("D");
	s_map[0x21] = _T("F");
	s_map[0x22] = _T("G");
	s_map[0x23] = _T("H");
	s_map[0x24] = _T("J");
	s_map[0x25] = _T("K");
	s_map[0x26] = _T("L");
	s_map[0x27] = _T("SEMICOLON");    // 分号 ;
	s_map[0x28] = _T("APOSTROPHE");   // 单引号 '
	s_map[0x29] = _T("GRAVE");        // accent grave `
	s_map[0x2A] = _T("LSHIFT");       // 左 SHIFT
	s_map[0x2B] = _T("BACKSLASH");    // 反斜杠  ＼
	s_map[0x2C] = _T("Z");
	s_map[0x2D] = _T("X");
	s_map[0x2E] = _T("C");
	s_map[0x2F] = _T("V");
	s_map[0x30] = _T("B");
	s_map[0x31] = _T("N");
	s_map[0x32] = _T("M");
	s_map[0x33] = _T("COMMA");        // 逗号 ,
	s_map[0x34] = _T("PERIOD");       // 点号 . on main keyboard
	s_map[0x35] = _T("SLASH");        // 斜杠 / on main keyboard
	s_map[0x36] = _T("RSHIFT");
	s_map[0x37] = _T("MULTIPLY");     // 星号 * on numeric keypad
	s_map[0x38] = _T("LMENU");        // 左 Alt
	s_map[0x39] = _T("SPACE");
	s_map[0x3A] = _T("CAPITAL");      // Capslock
	s_map[0x3B] = _T("F1");
	s_map[0x3C] = _T("F2");
	s_map[0x3D] = _T("F3");
	s_map[0x3E] = _T("F4");
	s_map[0x3F] = _T("F5");
	s_map[0x40] = _T("F6");
	s_map[0x41] = _T("F7");
	s_map[0x42] = _T("F8");
	s_map[0x43] = _T("F9");
	s_map[0x44] = _T("F10");
	s_map[0x45] = _T("NUMLOCK");
	s_map[0x46] = _T("SCROLL");       // Scroll Lock
	s_map[0x47] = _T("NUMPAD7");
	s_map[0x48] = _T("NUMPAD8");
	s_map[0x49] = _T("NUMPAD9");
	s_map[0x4A] = _T("SUBTRACT");     // - on numeric keypad
	s_map[0x4B] = _T("NUMPAD4");
	s_map[0x4C] = _T("NUMPAD5");
	s_map[0x4D] = _T("NUMPAD6");
	s_map[0x4E] = _T("ADD");          // + on numeric keypad
	s_map[0x4F] = _T("NUMPAD1");
	s_map[0x50] = _T("NUMPAD2");
	s_map[0x51] = _T("NUMPAD3");
	s_map[0x52] = _T("NUMPAD0");
	s_map[0x53] = _T("DECIMAL");      // . on numeric keypad
	s_map[0x56] = _T("OEM_102");      // <> or \| on RT 102-key keyboard (Non-U.S.)
	s_map[0x57] = _T("F11");
	s_map[0x58] = _T("F12");
	s_map[0x64] = _T("F13");          // (NEC PC98)
	s_map[0x65] = _T("F14");          // (NEC PC98)
	s_map[0x66] = _T("F15");          // (NEC PC98)
	s_map[0x70] = _T("KANA");         // (Japanese keyboard)
	s_map[0x73] = _T("ABNT_C1");      // /? on Brazilian keyboard
	s_map[0x79] = _T("CONVERT");      // (Japanese keyboard)
	s_map[0x7B] = _T("NOCONVERT");    // (Japanese keyboard)
	s_map[0x7D] = _T("YEN");          // (Japanese keyboard)
	s_map[0x7E] = _T("ABNT_C2");      // Numpad . on Brazilian keyboard
	s_map[0x8D] = _T("NUMPADEQUALS"); // = on numeric keypad (NEC PC98)
	s_map[0x90] = _T("PREVTRACK");    // Previous Track (DIK_CIRCUMFLEX on Japanese keyboard)
	s_map[0x91] = _T("AT");           // (NEC PC98)
	s_map[0x92] = _T("COLON");        // (NEC PC98)
	s_map[0x93] = _T("UNDERLINE");    // (NEC PC98)
	s_map[0x94] = _T("KANJI");        // (Japanese keyboard)
	s_map[0x95] = _T("STOP");         // (NEC PC98)
	s_map[0x96] = _T("AX");           // (Japan AX)
	s_map[0x97] = _T("UNLABELED");    // (J3100)
	s_map[0x99] = _T("NEXTTRACK");    // Next Track
	s_map[0x9C] = _T("NUMPADENTER");  // Enter on numeric keypad
	s_map[0x9D] = _T("RCONTROL");
	s_map[0xA0] = _T("MUTE");         // Mute
	s_map[0xA1] = _T("CALCULATOR");   // Calculator
	s_map[0xA2] = _T("PLAYPAUSE");    // Play / Pause
	s_map[0xA4] = _T("MEDIASTOP");    // Media Stop
	s_map[0xAE] = _T("VOLUMEDOWN");   // Volume -
	s_map[0xB0] = _T("VOLUMEUP");     // Volume +
	s_map[0xB2] = _T("WEBHOME");      // Web home
	s_map[0xB3] = _T("NUMPADCOMMA");  // , on numeric keypad (NEC PC98)
	s_map[0xB5] = _T("DIVIDE");       // / on numeric keypad
	s_map[0xB7] = _T("SYSRQ");
	s_map[0xB8] = _T("RMENU");        // 右 Alt
	s_map[0xC5] = _T("PAUSE");        // Pause
	s_map[0xC7] = _T("HOME");         // Home on arrow keypad
	s_map[0xC8] = _T("UP");           // ↑
	s_map[0xC9] = _T("PRIOR");        // PgUp
	s_map[0xCB] = _T("LEFT");         // ←
	s_map[0xCD] = _T("RIGHT");        // →
	s_map[0xCF] = _T("END");          // End
	s_map[0xD0] = _T("DOWN");         // ↓
	s_map[0xD1] = _T("NEXT");         // PgDn
	s_map[0xD2] = _T("INSERT");       // Insert
	s_map[0xD3] = _T("DELETE");       // Delete
	s_map[0xDB] = _T("LWIN");         // 左 Windows 徽标键
	s_map[0xDC] = _T("RWIN");         // 右 Windows 徽标键
	s_map[0xDD] = _T("APPS");         // AppMenu key
	s_map[0xDE] = _T("POWER");        // System Power
	s_map[0xDF] = _T("SLEEP");        // System Sleep
	s_map[0xE3] = _T("WAKE");         // System Wake
	s_map[0xE5] = _T("WEBSEARCH");    // Web Search
	s_map[0xE6] = _T("WEBFAVORITES"); // Web Favorites
	s_map[0xE7] = _T("WEBREFRESH");   // Web Refresh
	s_map[0xE8] = _T("WEBSTOP");      // Web Stop
	s_map[0xE9] = _T("WEBFORWARD");   // Web Forward
	s_map[0xEA] = _T("WEBBACK");      // Web Back
	s_map[0xEB] = _T("MYCOMPUTER");   // My Computer
	s_map[0xEC] = _T("MAIL");         // Mail
	s_map[0xED] = _T("MEDIASELECT");  // Media Select
}

