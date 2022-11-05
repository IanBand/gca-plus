#include <iostream>
#include <mutex>
#include <future>
#include <atomic>
#include <bitset>
#include <iomanip>
#include <algorithm>

#include "GCAdapter.h"

#include "Flag.h"

using namespace std;

libusb_device_handle *device_handle = nullptr;
libusb_context *context = nullptr;

uint8_t controller_payload[37];
uint8_t controller_payload_swap[37];

atomic<int> controller_payload_size = { 0 };

thread adapter_thread;
GCAdapter::Flag adapter_thread_running;

mutex main_mutex;

uint8_t endpoint_out = 0;
uint8_t endpoint_in = 0;

namespace gca {

	int Setup() {
		int return_value = 0;
		libusb_init(&context);
		libusb_device **list;
		ssize_t count = libusb_get_device_list(context, &list);
		bool case_test = false;
		for (int i = 0; i < count && !case_test; i++) {
			libusb_device *device = list[i];
			case_test = IsAccessible(device);
			if (case_test) {
				return_value = AddAdapter(device);
			}
		}
		libusb_free_device_list(list, 1);
		return return_value;
	}
	bool IsAccessible(libusb_device *dev) {
		int return_value = 0, kernel_value = 0;
		libusb_device_descriptor descriptor;
		return_value = libusb_get_device_descriptor(dev, &descriptor);
		if (return_value < 0) {
			cout << "Error getting descriptor of USB device. Error code: " << return_value << endl;
			return false;
		}

		if (descriptor.idVendor == GAMECUBE_VID && descriptor.idProduct == GAMECUBE_PID) {
			cout << "Found GameCube Adapter" << endl;
		}
		else {
			return false;
		}
		return_value = libusb_open(dev, &device_handle);
		switch (return_value) {
		case 0:
			cout << "This adapter seems reachable. Trying to reach..." << endl;
			break;
		case LIBUSB_ERROR_ACCESS:
			cout << "LIBUSB_ERROR_ACCESS: gca-node does not have access to this adapter." << endl;
			return false;
			break;
		default:
			cout << "gca-node couldn't open this adapter. Error code: " << return_value << endl;
			return false;
			break;
		}
		return_value = libusb_kernel_driver_active(device_handle, 0);
		if (return_value == 1) {
			kernel_value = libusb_detach_kernel_driver(device_handle, 0);
			if (kernel_value != 0) {
				cout << "gca-node can't attach the kernel of this adapter. Error code:" << return_value << endl;
				return false;
			}
		}
		if (return_value == 0 || kernel_value == 0) {
			return_value = libusb_claim_interface(device_handle, 0);
			if (return_value != 0) {
				cout << "gca-node couldn't claim interface 0 of adapter. Error code:" << return_value << endl;
			}
			return return_value == 0;
		}
		return false;
	}
	int AddAdapter(libusb_device *dev) {
		int endpoint_number = 0;
		libusb_config_descriptor *config = nullptr;
		libusb_get_config_descriptor(dev, 0, &config);
		for (uint8_t iface = 0; iface < config->bNumInterfaces; iface++) {
			const libusb_interface *interfaceContainer = &config->interface[iface];
			for (int i = 0; i < interfaceContainer->num_altsetting; i++) {
				const libusb_interface_descriptor *idesc = &interfaceContainer->altsetting[i];
				for (uint8_t epoint = 0; epoint < idesc->bNumEndpoints; epoint++) {
					const libusb_endpoint_descriptor *endpoint = &idesc->endpoint[epoint];
					endpoint_number++;
					if (endpoint->bEndpointAddress & LIBUSB_ENDPOINT_IN) {
						endpoint_in = endpoint->bEndpointAddress;
					}
					else {
						endpoint_out = endpoint->bEndpointAddress;
					}
				}
			}
		}

		return endpoint_number;
	}
	int Load() {
		int payload_size = 0, tmp = 0;
		uint8_t payload = 0x13;
		return libusb_interrupt_transfer(device_handle, endpoint_out, &payload, sizeof(payload), &tmp, 16);
	}
	int Stop() {
		int code = 0;
		if (adapter_thread_running.TestAndClear()) {
			adapter_thread.join();
		}
		if (device_handle) {
			code = libusb_release_interface(device_handle, 0);
			libusb_close(device_handle);
			device_handle = nullptr;
		}
		return code;
	}
	void Read() {
		int payload_size = 0;
		int code = libusb_interrupt_transfer(device_handle, endpoint_in, controller_payload_swap, sizeof(controller_payload_swap), &payload_size, 16);

		lock_guard<mutex> lock(main_mutex);
		swap(controller_payload_swap, controller_payload);
		controller_payload_size.store(payload_size);

		this_thread::yield();
	}

	unsigned int GetNthBit(uint8_t number, int n) {
		unsigned int bit = (unsigned)(number & (1 << n - 1));
		return bit >> n - 1;
	}
	void Process(ControllerStatus* buffer) {
		adapter_thread_running.Set(true);
		adapter_thread = thread(Read);
		if (adapter_thread_running.TestAndClear()) {
			adapter_thread.join();
		}
		for (int i = 0; i < 4; i++) {
			buffer[i] = GetGamepadStatus(controller_payload, i);
		}
	}
	string RawData() {
		adapter_thread_running.Set(true);
		adapter_thread = thread(Read);
		if (adapter_thread_running.TestAndClear()) {
			adapter_thread.join();
		}
		stringstream return_value;
		for (int i = 0; i < 37; i++) {

			return_value << i << "_______";
			if(i < 10){ // i is 1 digit, insert extra
				return_value << "_";
			}
		}
		return_value << "\n";
		for (int i = 0; i < 37; i++) {
			return_value << bitset<8>(controller_payload[i]) << ",";
		}
		return return_value.str();
	}

	ControllerStatus GetGamepadStatus(uint8_t * controller_payload, int i) {
		ControllerStatus status;

		status.connected   = GetNthBit(controller_payload[1 + i * 9], 5); // occurs in payload bytes: 1, 10, 19, 28

		status.buttonA     = GetNthBit(controller_payload[2 + i * 9], 1);
		status.buttonB     = GetNthBit(controller_payload[2 + i * 9], 2);
		status.buttonX     = GetNthBit(controller_payload[2 + i * 9], 3);
		status.buttonY     = GetNthBit(controller_payload[2 + i * 9], 4);

		status.padLeft     = GetNthBit(controller_payload[2 + i * 9], 5);
		status.padRight    = GetNthBit(controller_payload[2 + i * 9], 6);
		status.padDown     = GetNthBit(controller_payload[2 + i * 9], 7);
		status.padUp       = GetNthBit(controller_payload[2 + i * 9], 8);

		status.buttonStart = GetNthBit(controller_payload[3 + i * 9], 1);
		status.buttonZ     = GetNthBit(controller_payload[3 + i * 9], 2);
		status.buttonR     = GetNthBit(controller_payload[3 + i * 9], 3);
		status.buttonL     = GetNthBit(controller_payload[3 + i * 9], 4);

		// TODO: memcopy these? memcopy all?
		// TODO: add back bias correction & controller_analog_bias struct?

		status.mainStickHorizontal = controller_payload[4 + i * 9];
		status.mainStickVertical   = controller_payload[5 + i * 9];

		status.cStickHorizontal    = controller_payload[6 + i * 9];
		status.cStickVertical      = controller_payload[7 + i * 9];

		status.triggerL            = controller_payload[8 + i * 9];
		status.triggerR            = controller_payload[9 + i * 9];

		return status;
	}
}