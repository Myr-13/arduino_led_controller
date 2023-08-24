#include "serial_manager.h"

#include <chrono>

CSerialManager::CSerialManager()
{
	m_ThreadRunning = true;
	m_Connect = false;
	m_State = SM_STATE_NOT_CONNECTED;
	m_BaudRate = 9600;

	m_Port.resize(8);
	m_Port = "COM3";

	m_Thread = std::thread([&]() { ThreadFunc(); });
}

CSerialManager::~CSerialManager()
{
	m_ThreadRunning = false;
	m_Thread.join();
}

void CSerialManager::ThreadFunc()
{
	ceSerial Com;
	Com.SetParity('N');
	Com.SetDataSize(8);
	Com.SetStopBits(1);

	while(m_ThreadRunning)
	{
		// Connect
		if(!Com.IsOpened() && m_Connect)
		{
			Com.SetBaudRate(m_BaudRate);
			Com.SetPortName(std::string(R"(\\.\)") + m_Port);

			if(Com.Open() == 0)
				m_State = SM_STATE_CONNECTED;
		}

		// Send data
		if(Com.IsOpened() && m_Connect)
		{
			// Pack some data and send to comm port
			uint8_t aBuf[16];
			aBuf[0] = m_Data.m_Mode;
			aBuf[1] = m_Data.m_Bright;
			aBuf[2] = m_Data.m_Speed;
			aBuf[3] = m_Data.m_Temperature;
			aBuf[4] = (uint8_t)(m_Data.m_CustomColor[0] * 255);
			aBuf[5] = (uint8_t)(m_Data.m_CustomColor[1] * 255);
			aBuf[6] = (uint8_t)(m_Data.m_CustomColor[2] * 255);

			Com.Write(aBuf, 16);
		}

		// Closing
		if(Com.IsOpened() && !m_Connect)
		{
			Com.Close();
			m_State = SM_STATE_NOT_CONNECTED;
		}

		// Sleep 100ms
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

void CSerialManager::Open()
{
	m_Connect = true;
}

void CSerialManager::Close()
{
	m_Connect = false;
}
