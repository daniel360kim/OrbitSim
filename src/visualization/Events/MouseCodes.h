/**
 * @file MouseCodes.h
 * @author Daniel Kim (daniel.kim@studentlaschools.net)
 * @brief mouse codes
 * @version 0.1
 * @date 2023-06-24
 * 
 * 
 */

#ifndef MOUSECODES_H
#define MOUSECODES_H

#include <cstdint>

using MouseCode = uint16_t;

	namespace Mouse
	{
		enum : MouseCode
		{
			// From glfw3.h
			Button0                = 0,
			Button1                = 1,
			Button2                = 2,
			Button3                = 3,
			Button4                = 4,
			Button5                = 5,
			Button6                = 6,
			Button7                = 7,

			ButtonLast             = Button7,
			ButtonLeft             = Button0,
			ButtonRight            = Button1,
			ButtonMiddle           = Button2
		};
	}

#endif