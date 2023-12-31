#include "window.h"

#include <fstream>

CWindow::CWindow(unsigned int W, unsigned int H)
{
	m_Quiting = false;
	m_Visible = false;

	// Create window
	m_Window.create({W, H}, "LED Controller");
	m_Window.setVerticalSyncEnabled(true);
	m_Window.setFramerateLimit(60);

	ImGui::SFML::Init(m_Window);

	// Config
	LoadConfig();

	// Hide window
	//Hide();
}

void CWindow::Update()
{
	sf::Event event {};

	while(m_Window.pollEvent(event))
	{
		ImGui::SFML::ProcessEvent(event);

		if(event.type == sf::Event::Closed)
			Hide();
	}

	sf::Time DeltaTime = m_DeltaClock.restart();
	ImGui::SFML::Update(m_Window, DeltaTime);

	SSerialData &Data = m_Serial.m_Data;

	ImGui::Begin("Main");
	ImGui::Text("Serial info:");
	ImGui::SliderInt("Mode", &Data.m_Mode, 0, NUM_MODES - 1);
	ImGui::SliderInt("Bright", &Data.m_Bright, 0, 255);
	if(!m_OnlySupport || Data.m_Mode == MODE_RAINBOW)
		ImGui::SliderInt("Speed", &Data.m_Speed, 1, 255);
	if(!m_OnlySupport || Data.m_Mode == MODE_WHITE)
		ImGui::SliderInt("Temperature", &Data.m_Temperature, 0, 255);
	if(!m_OnlySupport || Data.m_Mode == MODE_CUSTOM)
		ImGui::ColorEdit3("Custom color", Data.m_CustomColor);
	ImGui::Text("Other:");
	m_Quiting = ImGui::Button("Exit");
	ImGui::Checkbox("Show only supported settings for current mode", &m_OnlySupport);
	ImGui::End();

	static const char *s_apItems[] = {"9600", "19200", "38400", "57600", "115200", "230400", "460800", "921600", "1000000", "2000000"};
	static int s_aBauds[] = {9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600, 1000000, 2000000};
	int SelectedItem = 0;

	ImGui::Begin("Port");
	ImGui::InputText("Port name", m_Serial.m_Port.data(), 8);
	if(ImGui::Combo("Baud rate", &SelectedItem, s_apItems, 10))
		m_Serial.m_BaudRate = s_aBauds[SelectedItem];
	if(m_Serial.m_State == SM_STATE_NOT_CONNECTED && ImGui::Button("Open"))
		m_Serial.Open();
	if(m_Serial.m_State == SM_STATE_CONNECTED && ImGui::Button("Close"))
		m_Serial.Close();
	ImGui::End();

	m_Window.clear();
	ImGui::SFML::Render(m_Window);
	m_Window.display();
}

void CWindow::SaveConfig()
{
	std::ofstream File("config.txt");
	if(!File)
		return;

	File << m_Serial.m_Data.m_Mode << " ";
	File << m_Serial.m_Data.m_Bright << " ";
	File << m_Serial.m_Data.m_Speed << " ";
	File << m_Serial.m_Data.m_Temperature << " ";
	File << (int)(m_Serial.m_Data.m_CustomColor[0] * 255) << " ";
	File << (int)(m_Serial.m_Data.m_CustomColor[1] * 255) << " ";
	File << (int)(m_Serial.m_Data.m_CustomColor[2] * 255) << " ";
	File << m_Serial.m_Port << " ";
	File << (int)m_OnlySupport << " ";

	File.close();
}

void CWindow::LoadConfig()
{
	std::ifstream File("config.txt");
	if(!File)
		return;

	std::string Temp;
	File >> Temp; m_Serial.m_Data.m_Mode = std::stoi(Temp);
	File >> Temp; m_Serial.m_Data.m_Bright = std::stoi(Temp);
	File >> Temp; m_Serial.m_Data.m_Speed = std::stoi(Temp);
	File >> Temp; m_Serial.m_Data.m_Temperature = std::stoi(Temp);
	File >> Temp; m_Serial.m_Data.m_CustomColor[0] = (float)std::stoi(Temp) / 255;
	File >> Temp; m_Serial.m_Data.m_CustomColor[1] = (float)std::stoi(Temp) / 255;
	File >> Temp; m_Serial.m_Data.m_CustomColor[2] = (float)std::stoi(Temp) / 255;
	File >> m_Serial.m_Port;
	File >> Temp; m_OnlySupport = (bool)std::stoi(Temp);

	File.close();
}

void CWindow::Close()
{
	m_Window.close();
	m_Serial.Delete();
	SaveConfig();
}

void CWindow::ToggleVisible()
{
	if(m_Visible)
		Hide();
	else
		Show();
}

void CWindow::Show()
{
	m_Window.setVisible(true);
	m_Window.setActive(true);

	m_Visible = true;
}

void CWindow::Hide()
{
	m_Window.setVisible(false);
	m_Window.setActive(false);

	m_Visible = false;
}
