#pragma once
#include <iostream>

using namespace std;

class ControllerStatus {
public:
	bool connected;
	bool buttonA, buttonB, buttonX, buttonY;
	bool padLeft, padRight, padUp, padDown;
	bool buttonL, buttonR, buttonZ, buttonStart;

	int mainStickHorizontal, mainStickVertical;
	int cStickHorizontal, cStickVertical;

	int triggerL, triggerR;
};
inline ostream& operator<<(ostream & string, ControllerStatus const &v) {
	string << "{ connected: " << v.connected
		<< ", buttonA: " << v.buttonA << ", buttonB: " << v.buttonB << ", buttonX: " << v.buttonX << ", buttonY: " << v.buttonY
		<< ", buttonL: " << v.buttonL << ", buttonR: " << v.buttonR << ", buttonZ: " << v.buttonZ << ", buttonSTART: " << v.buttonStart
		<< ", mainStickHorizontal: " << v.mainStickHorizontal << ", mainStickVertical:" << v.mainStickVertical
		<< ", cStickHorizontal: " << v.cStickHorizontal << ", cStickVerticalAxis: " << v.cStickVertical
		<< ", triggerL: " << v.triggerL << endl << ", triggerR:" << v.triggerR << " }";
	return string;
};
inline ostream& operator<=(ostream & string, ControllerStatus const &v) {
	string << "{ connected: " << v.connected
		<< ", buttonA: " << v.buttonA << ", buttonB: " << v.buttonB << ", buttonX: " << v.buttonX << ", buttonY: " << v.buttonY
		<< ", buttonL: " << v.buttonL << ", buttonR: " << v.buttonR << ", buttonZ: " << v.buttonZ << ", buttonSTART: " << v.buttonStart
		<< ", mainStickHorizontal: " << v.mainStickHorizontal << ", mainStickVertical:" << v.mainStickVertical
		<< ", cStickHorizontal: " << v.cStickHorizontal << ", cStickVerticalAxis: " << v.cStickVertical
		<< ", triggerL: " << v.triggerL << endl << ", triggerR:" << v.triggerR << " }";
	return string;
}
