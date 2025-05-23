#pragma once

// from glfw3.h

namespace BrokenSim
{
	using KeyCode = unsigned int;

	namespace Key
	{
		enum : KeyCode
		{
			// Printable keys
			Space					= 32,
			Apostrophe			    = 39,  /* ' */
			Comma				    = 44,  /* , */
			Minus				    = 45,  /* - */
			Period				    = 46,  /* . */
			Slash				    = 47,  /* / */
			Key_0				    = 48,
			Key_1				    = 49,
			Key_2				    = 50,
			Key_3				    = 51,
			Key_4				    = 52,
			Key_5				    = 53,
			Key_6				    = 54,
			Key_7				    = 55,
			Key_8				    = 56,
			Key_9				    = 57,
			Semicolon			    = 59,  /* ; */
			Equal				    = 61,  /* = */
			A					    = 65,
			B					    = 66,
			C					    = 67,
			D					    = 68,
			E					    = 69,
			F					    = 70,
			G					    = 71,
			H					    = 72,
			I					    = 73,
			J					    = 74,
			K					    = 75,
			L					    = 76,
			M					    = 77,
			N					    = 78,
			O					    = 79,
			P					    = 80,
			Q					    = 81,
			R					    = 82,
			S					    = 83,
			T					    = 84,
			U					    = 85,
			V					    = 86,
			W					    = 87,
			X					    = 88,
			Y					    = 89,
			Z					    = 90,
			LeftBracket			    = 91,  /* [ */
			BackSlash			    = 92,  /* \ */
			RightBracket		    = 93,  /* ] */
			GraveAccent			    = 96,  /* ` */
			World_1				    = 161, /* non-US #1 */
			World_2				    = 162, /* non-US #2 */

			// Function keys
			Escape					= 256,
			Enter					= 257,
			Tab						= 258,
			Backspace				= 259,
			Insert					= 260,
			Delete					= 261,
			Right					= 262,
			Left					= 263,
			Down					= 264,
			Up						= 265,
			PageUp					= 266,
			PageDown				= 267,
			Home					= 268,
			End						= 269,
			CapsLock				= 280,
			ScrollLock				= 281,
			NumLock					= 282,
			PrintScreen				= 283,
			Pause					= 284,
			F1						= 290,
			F2						= 291,
			F3						= 292,
			F4						= 293,
			F5						= 294,
			F6						= 295,
			F7						= 296,
			F8						= 297,
			F9						= 298,
			F10						= 299,
			F11						= 300,
			F12						= 301,
			F13						= 302,
			F14						= 303,
			F15						= 304,
			F16						= 305,
			F17						= 306,
			F18						= 307,
			F19						= 308,
			F20						= 309,
			F21						= 310,
			F22						= 311,
			F23						= 312,
			F24						= 313,
			F25						= 314,
			KP_0					= 320,
			KP_1					= 321,
			KP_2					= 322,
			KP_3					= 323,
			KP_4					= 324,
			KP_5					= 325,
			KP_6					= 326,
			KP_7					= 327,
			KP_8					= 328,
			KP_9					= 329,
			KP_Decimal				= 330,
			KP_Divide				= 331,
			KP_Multiply				= 332,
			KP_Subtract				= 333,
			KP_Add					= 334,
			KP_Enter				= 335,
			KP_Equal				= 336,
			LeftShift				= 340,
			LeftControl				= 341,
			LeftAlt					= 342,
			LeftSuper				= 343,
			RightShift				= 344,
			RightControl			= 345,
			RightAlt				= 346,
			RightSuper				= 347,
			Menu					= 348
		};
	}
}
