#pragma once

#include "common.h"

namespace GRAVEngine
{
	namespace Events
	{
		enum eventCategory
		{
			none = 0,
			eventCategoryApplication	= BIT(0),
			eventCategoryInput			= BIT(1),
			eventCategoryKeyboard		= BIT(2),
			eventCategoryMouse			= BIT(3),
			eventCategoryMouseButton	= BIT(4)
		};
	}
}