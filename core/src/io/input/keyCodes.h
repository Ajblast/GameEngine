#pragma once

#include "common.h"
#include <array>

namespace GRAVEngine
{
	namespace Keys
	{
		typedef uint16 keyCode;

		// WARNING! Any changes to this enum requires a change in the coresponding iterable keycode array

		// A key
		enum : keyCode
		{
			// From glfw3.h
			Space = 32,
			Apostrophe = 39, /* ' */
			Comma = 44, /* , */
			Minus = 45, /* - */
			Period = 46, /* . */
			Slash = 47, /* / */

			D0 = 48, /* 0 */
			D1 = 49, /* 1 */
			D2 = 50, /* 2 */
			D3 = 51, /* 3 */
			D4 = 52, /* 4 */
			D5 = 53, /* 5 */
			D6 = 54, /* 6 */
			D7 = 55, /* 7 */
			D8 = 56, /* 8 */
			D9 = 57, /* 9 */

			Semicolon = 59, /* ; */
			Equal = 61, /* = */

			A = 65,
			B = 66,
			C = 67,
			D = 68,
			E = 69,
			F = 70,
			G = 71,
			H = 72,
			I = 73,
			J = 74,
			K = 75,
			L = 76,
			M = 77,
			N = 78,
			O = 79,
			P = 80,
			Q = 81,
			R = 82,
			S = 83,
			T = 84,
			U = 85,
			V = 86,
			W = 87,
			X = 88,
			Y = 89,
			Z = 90,

			LeftBracket = 91,  /* [ */
			Backslash = 92,  /* \ */
			RightBracket = 93,  /* ] */
			GraveAccent = 96,  /* ` */

			World1 = 161, /* non-US #1 */
			World2 = 162, /* non-US #2 */

			/* Function keys */
			Escape = 256,
			Enter = 257,
			Tab = 258,
			Backspace = 259,
			Insert = 260,
			Delete = 261,
			Right = 262,
			Left = 263,
			Down = 264,
			Up = 265,
			PageUp = 266,
			PageDown = 267,
			Home = 268,
			End = 269,
			CapsLock = 280,
			ScrollLock = 281,
			NumLock = 282,
			PrintScreen = 283,
			Pause = 284,
			F1 = 290,
			F2 = 291,
			F3 = 292,
			F4 = 293,
			F5 = 294,
			F6 = 295,
			F7 = 296,
			F8 = 297,
			F9 = 298,
			F10 = 299,
			F11 = 300,
			F12 = 301,
			F13 = 302,
			F14 = 303,
			F15 = 304,
			F16 = 305,
			F17 = 306,
			F18 = 307,
			F19 = 308,
			F20 = 309,
			F21 = 310,
			F22 = 311,
			F23 = 312,
			F24 = 313,
			F25 = 314,

			/* Keypad */
			KP0 = 320,
			KP1 = 321,
			KP2 = 322,
			KP3 = 323,
			KP4 = 324,
			KP5 = 325,
			KP6 = 326,
			KP7 = 327,
			KP8 = 328,
			KP9 = 329,
			KPDecimal = 330,
			KPDivide = 331,
			KPMultiply = 332,
			KPSubtract = 333,
			KPAdd = 334,
			KPEnter = 335,
			KPEqual = 336,

			LeftShift = 340,
			LeftControl = 341,
			LeftAlt = 342,
			LeftSuper = 343,
			RightShift = 344,
			RightControl = 345,
			RightAlt = 346,
			RightSuper = 347,
			Menu = 348
		};

		// Iterable key codes
		const std::array<keyCode, 120> g_KeyCodes = {
			// From glfw3.h
			Keys::Space,
			Keys::Apostrophe,	/* ' */
			Keys::Comma,		/* , */
			Keys::Minus,		/* - */
			Keys::Period,		/* . */
			Keys::Slash,		/* / */
			Keys::D0, /* 0 */
			Keys::D1, /* 1 */
			Keys::D2, /* 2 */
			Keys::D3, /* 3 */
			Keys::D4, /* 4 */
			Keys::D5, /* 5 */
			Keys::D6, /* 6 */
			Keys::D7, /* 7 */
			Keys::D8, /* 8 */
			Keys::D9, /* 9 */
			Keys::Semicolon,	/* ; */
			Keys::Equal,		/* = */
			Keys::A,
			Keys::B,
			Keys::C,
			Keys::D,
			Keys::E,
			Keys::F,
			Keys::G,
			Keys::H,
			Keys::I,
			Keys::J,
			Keys::K,
			Keys::L,
			Keys::M,
			Keys::N,
			Keys::O,
			Keys::P,
			Keys::Q,
			Keys::R,
			Keys::S,
			Keys::T,
			Keys::U,
			Keys::V,
			Keys::W,
			Keys::X,
			Keys::Y,
			Keys::Z,
			Keys::LeftBracket,  /* [ */
			Keys::Backslash,	/* \ */
			Keys::RightBracket, /* ] */
			Keys::GraveAccent,  /* ` */
			Keys::World1, /* non-US #1 */
			Keys::World2, /* non-US #2 */
			/* Function keys */
			Keys::Escape,
			Keys::Enter,
			Keys::Tab,
			Keys::Backspace,
			Keys::Insert,
			Keys::Delete,
			Keys::Right,
			Keys::Left,
			Keys::Down,
			Keys::Up,
			Keys::PageUp,
			Keys::PageDown,
			Keys::Home,
			Keys::End,
			Keys::CapsLock,
			Keys::ScrollLock,
			Keys::NumLock,
			Keys::PrintScreen,
			Keys::Pause,
			Keys::F1,
			Keys::F2,
			Keys::F3,
			Keys::F4,
			Keys::F5,
			Keys::F6,
			Keys::F7,
			Keys::F8,
			Keys::F9,
			Keys::F10,
			Keys::F11,
			Keys::F12,
			Keys::F13,
			Keys::F14,
			Keys::F15,
			Keys::F16,
			Keys::F17,
			Keys::F18,
			Keys::F19,
			Keys::F20,
			Keys::F21,
			Keys::F22,
			Keys::F23,
			Keys::F24,
			Keys::F25,
			/* Keypad */
			Keys::KP0,
			Keys::KP1,
			Keys::KP2,
			Keys::KP3,
			Keys::KP4,
			Keys::KP5,
			Keys::KP6,
			Keys::KP7,
			Keys::KP8,
			Keys::KP9,
			Keys::KPDecimal,
			Keys::KPDivide,
			Keys::KPMultiply,
			Keys::KPSubtract,
			Keys::KPAdd,
			Keys::KPEnter,
			Keys::KPEqual,
			Keys::LeftShift,
			Keys::LeftControl,
			Keys::LeftAlt,
			Keys::LeftSuper,
			Keys::RightShift,
			Keys::RightControl,
			Keys::RightAlt,
			Keys::RightSuper,
			Keys::Menu
		};

	}
}