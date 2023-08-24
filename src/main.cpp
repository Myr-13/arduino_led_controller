#include <iostream>

#define CE_SERIAL_IMPLEMENTATION
#include "serial.h"
#include "window.h"

#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

int main() {
    CWindow win(600, 600);

    bool old_pressed = false;
	while(!win.m_Quiting) {
		// Key bind for show/hide window
		bool pressed = GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState(VK_LWIN) && GetAsyncKeyState(VK_F10);
		if (pressed && !old_pressed)
			win.ToggleVisible();

		old_pressed = pressed;

		win.Update();
	}

	win.Close();

	return 0;
}
