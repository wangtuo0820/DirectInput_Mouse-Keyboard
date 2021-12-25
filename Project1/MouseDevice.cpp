#include "MouseDevice.h"

MouseDevice::MouseDevice(HWND hWnd, LPDIRECTINPUT8 pDirectInput) : m_lasterror(), m_isLastReadingFailed(false) {
	HRESULT hr;
	m_hWnd = hWnd;
	m_buffer = new DIMOUSESTATE;
	ZeroMemory(m_buffer, sizeof(m_buffer));
	hr = pDirectInput->CreateDevice(GUID_SysMouse, &m_ms, NULL);
	if (!setError(hr, _T("CreateDevice"))) {
		hr = m_ms->SetDataFormat(&c_dfDIMouse);
		if (!setError(hr, _T("SetDataFormat"))) {
			hr = m_ms->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
			setError(hr, _T("SetCooperativeLevel"));
		}
	}
}

bool MouseDevice::dev_read() {
	HRESULT hr;

	m_ms->Poll();
	m_ms->Acquire();

	if (SUCCEEDED(hr = m_ms->GetDeviceState(sizeof(*m_buffer), m_buffer)))
		return true;
	if (hr != DIERR_INPUTLOST && hr != DIERR_NOTACQUIRED) return false;
	if (FAILED(m_ms->Acquire())) return false;

	return false;
}

bool MouseDevice::read() {
	Sleep(7);
	if (dev_read()) {
		m_isLastReadingFailed = false;
	}
	else {
		m_isLastReadingFailed = true;
	}

	return !m_isLastReadingFailed;
}

MouseDevice::~MouseDevice() {

	if ((m_ms) != NULL) { (m_ms)->Release(); (m_ms) = NULL; }
}



//// private:

// return true if hr is failed
bool MouseDevice::setError(HRESULT hr, LPCTSTR func_name) {
	if (FAILED(hr)) {
		m_lasterror.set(hr, func_name);
		return true;
	}
	else
		return false;
}
