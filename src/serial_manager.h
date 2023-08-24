#ifndef LED_CONTROLLER_SERIAL_MANAGER_H
#define LED_CONTROLLER_SERIAL_MANAGER_H

#include <thread>
#include "serial.h"

struct SSerialData
{
	bool m_Sending = false;

	int m_Mode = 0;
	int m_Speed = 10;
	int m_Bright = 100;
	int m_Temperature = 0;
	float m_CustomColor[3] {1, 1, 1};
};

enum
{
	SM_STATE_NOT_CONNECTED,
	SM_STATE_CONNECTED
};

class CSerialManager {
	std::thread m_Thread;
	bool m_ThreadRunning;
	bool m_Connect;

	void ThreadFunc();

public:
	CSerialManager();

	void Open();
	void Close();
	void Delete();

	int m_BaudRate;
	std::string m_Port;

	int m_State;
	SSerialData m_Data;
};

#endif // LED_CONTROLLER_SERIAL_MANAGER_H
