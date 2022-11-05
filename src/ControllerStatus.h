#pragma once
#include <iostream>

using namespace std;

class ControllerStatus {
public:
	bool connected;
	bool button_a, button_b, button_x, button_y;
	bool pad_left, pad_right, padUp, padDown;
	bool button_l, button_r, button_z, button_start;

	int primary_x, primary_y;
	int secondary_x, secondary_y;

	int trigger_l, trigger_r;
};
inline ostream& operator<<(ostream & string, ControllerStatus const &v) {
	string << "{ connected: " << v.connected
		<< ", button_a: " << v.button_a << ", button_b: " << v.button_b << ", button_x: " << v.button_x << ", button_y: " << v.button_y
		<< ", button_l: " << v.button_l << ", button_r: " << v.button_r << ", button_z: " << v.button_z << ", button_start: " << v.button_start
		<< ", primary_x: " << v.primary_x << ", primary_y:" << v.primary_y
		<< ", secondary_x: " << v.secondary_x << ", secondary_y: " << v.secondary_y
		<< ", trigger_l: " << v.trigger_l << endl << ", trigger_r:" << v.trigger_r << " }";
	return string;
};
inline ostream& operator<=(ostream & string, ControllerStatus const &v) {
	string << "{ connected: " << v.connected
		<< ", button_a: " << v.button_a << ", button_b: " << v.button_b << ", button_x: " << v.button_x << ", button_y: " << v.button_y
		<< ", button_l: " << v.button_l << ", button_r: " << v.button_r << ", button_z: " << v.button_z << ", button_start: " << v.button_start
		<< ", primary_x: " << v.primary_x << ", primary_y:" << v.primary_y
		<< ", secondary_x: " << v.secondary_x << ", secondary_y: " << v.secondary_y
		<< ", trigger_l: " << v.trigger_l << endl << ", trigger_r:" << v.trigger_r << " }";
	return string;
}
