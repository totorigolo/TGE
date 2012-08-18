#pragma once
#include <Thor/Events.hpp>
#include <SFML/Window.hpp>

namespace thor
{
	class ActionMouseWheelUp : public Action
	{
	public:
		ActionMouseWheelUp();
		virtual ~ActionMouseWheelUp(void);
	};
}

namespace thor
{
	class ActionMouseWheelDown : public Action
	{
	public:
		ActionMouseWheelUp();
		virtual ~ActionMouseWheelDown(void);
	};
}
