#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>
#include <functional>

#include "enums.h"
#include "serial_manager.h"

// ImGui
#include "imgui.h"
#include "imgui-SFML.h"

class CWindow
{
	sf::RenderWindow m_Window;
	bool m_Visible;
	sf::Clock m_DeltaClock;
	CSerialManager m_Serial;

public:
	bool m_Quiting;

	CWindow(unsigned int W, unsigned int H);

	void Update();

	void ToggleVisible();
	void Show();
	void Hide();
};
