#pragma once

#define DIRECTINPUT_VERSION			0x0800

#include <windows.h>
#include <tchar.h>
#include <dinput.h>
#include <iostream>

using std::endl;
using std::cout;

class MouseDevice {
public:
	struct lasterror_t {
		HRESULT hr;
		LPCTSTR func_name;
		inline lasterror_t() {
			clear();
		}

		inline void set(HRESULT hr, LPCTSTR func_name) {
			this->hr = hr;
			this->func_name = func_name;
		}

		inline void clear() {
			hr = 0;
			func_name = NULL;
		}

		inline bool isFailed() const {
			return hr != 0;
		}
	};

	explicit MouseDevice(HWND hWnd, LPDIRECTINPUT8 pDirectInput);

	bool dev_read();

	bool read();

	inline lasterror_t getLastError() const {
		return m_lasterror;
	}

	inline DIMOUSESTATE* getState() {
		return m_buffer;
	}

	inline bool isLastReadingFailed() const {
		return m_isLastReadingFailed;
	}



	~MouseDevice();

private:
	// return true if hr has failed
	bool setError(HRESULT hr, LPCTSTR func_name);

	LPDIRECTINPUTDEVICE8 m_ms;
	lasterror_t m_lasterror;
	bool m_isLastReadingFailed;
	HWND m_hWnd;

	DIMOUSESTATE* m_buffer;
};
