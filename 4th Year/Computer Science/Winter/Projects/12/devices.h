// Kris Harper
// CMSC 15200
// 3.18.11
// Project 12

// I changed the name of resistance to R in the Resistor class
// so this won't work with the default mainRectifier.cpp.
// I think everything else should be fine though.
#ifndef DEVICESH
#define DEVICESH
#include <vector>
#include <string>
#include "matrix.h"
using std::string;
using std::vector;
typedef unsigned int uint;
typedef double (*funct)(double);

class Node;
class Device;
class CircuitState;

class Circuit {
	public:
		vector<Node*> Nodes;
		vector<Device*> Devices;
		uint stateLen;

		Circuit()
			{stateLen=0;}
		void addNode(Node* n);
		void addDevice(Device* d);
};

class Device {
	public:
		uint stateIndex;
		virtual void writeEqn(const CircuitState oldCS, double dt, fullMatrix& A, fullMatrix& RHS) = 0;
};

class OneEndedDevice : public Device {
	public:
		Node* base;
		string name;
		OneEndedDevice(Node* base, string name)
			: base(base), name(name) {}
};

class ExternalVoltage : public OneEndedDevice {
	public:
		funct f;
		ExternalVoltage(Circuit& cir, funct f, Node* n, string name);
		void writeEqn(const CircuitState oldCS, double dt, fullMatrix& A, fullMatrix& RHS);
};

class TwoEndedDevice : public Device {
	public:
		Node* base;
		Node* head;
		string name;
		TwoEndedDevice(Node* base, Node* head, string name)
			: base(base), head(head), name(name) {}
};

class Resistor : public TwoEndedDevice {
	public:
		double R;
		Resistor(Circuit& cir, double R, Node* n1, Node* n2, string name);
		void writeEqn(const CircuitState oldCS, double dt, fullMatrix& A, fullMatrix& RHS);
};

class Capacitor : public TwoEndedDevice {
	public:
		double C;
		Capacitor(Circuit& cir, double C, Node* n1, Node* n2, string name);
		void writeEqn(const CircuitState oldCS, double dt, fullMatrix& A, fullMatrix& RHS);
};

class Inductor : public TwoEndedDevice {
	public:
		double H;
		Inductor(Circuit& cir, double H, Node* n1, Node* n2, string name);
		void writeEqn(const CircuitState oldCS, double dt, fullMatrix& A, fullMatrix& RHS);
};

class Diode : public TwoEndedDevice {
	public:
		Diode(Circuit& cir, Node* n1, Node* n2, string name);
		void writeEqn(const CircuitState oldCS, double dt, fullMatrix& A, fullMatrix& RHS);
};

class Node {
	public:
		vector<Device*> base;
		vector<Device*> head;
		uint stateIndex;
		string name;
		Node(Circuit& cir, string name);
		void writeEqn(const CircuitState oldCS, double dt, fullMatrix& A, fullMatrix& RHS);
};

class CircuitState {
	public:
		Circuit* cir;
		fullMatrix soln;
		double time;
		CircuitState(Circuit& circuit);
		void zero();
};

void CircuitStep(CircuitState& state, CircuitState& next, double dt);

#endif
