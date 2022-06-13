#ifdef _WIN32
#include "libusb.h"
#else
#include <libusb-1.0/libusb.h>
#endif
#include <sstream>
#include "ControllerStatus.h"

#pragma once

#ifndef GC_CONTROLLER_NAMES
#define GC_CONTROLLER_NAMES

#define PORT_1 0
#define PORT_2 1
#define PORT_3 2
#define PORT_4 3

#endif

#pragma once

namespace gca {

	int AddAdapter(libusb_device *dev);
	int Load();
	int Setup();
	std::string Request();
	void Process(ControllerStatus*);
	int Stop();
	std::string RawData();
	void Read();
	bool IsAccessible(libusb_device *dev);
	std::string PollBytes(uint8_t *results);
	unsigned int GetNthBit(uint8_t number, int n);
	ControllerStatus GetGamepadStatus(uint8_t * results, int port);
	void ResetCalibration(int port);

	const uint16_t GAMECUBE_VID = 0x057E;
	const uint16_t GAMECUBE_PID = 0x0337;

	class ControllerMetadata {
		public:
		bool connected_on_prev_poll = false;
		int init_primary_x = 0;
		int init_primary_y = 0;
		int init_secondary_x = 0;
		int init_secondary_y = 0;
		int init_trigger_l = 0;
		int init_trigger_r = 0;
	};
}
