#include "devices.h"
#include <string>

void Circuit::addNode(Node* n)
{
	Nodes.push_back(n);
	n->stateIndex = stateLen++;
}

void Circuit::addDevice(Device* d)
{
	Devices.push_back(d);
	d->stateIndex = stateLen++;
}

Node::Node(Circuit& cir, string name)
: name(name)
{
	cir.addNode(this);
}

void Node::writeEqn(const CircuitState oldCS, double dt, fullMatrix& A, fullMatrix& RHS)
{
	uint i;
	double Ib = 0, Ih = 0;

	for(i=0; i < base.size(); i++){
		A(stateIndex, base[i]->stateIndex) = -1.0;
		Ib += oldCS.soln(base[i]->stateIndex);
	}
	for(i=0; i < head.size(); i++){
		A(stateIndex, head[i]->stateIndex) = 1.0;
		Ih += oldCS.soln(head[i]->stateIndex);
	}
	RHS(stateIndex) = Ib - Ih;
}

Inductor::Inductor(Circuit& cir, double H, Node* n1, Node* n2, string name)
: TwoEndedDevice(n1, n2, name), H(H)
{
	cir.addDevice(this);
	n1->base.push_back(this);
	n2->head.push_back(this);
}

void Inductor::writeEqn(const CircuitState oldCS, double dt, fullMatrix& A, fullMatrix& RHS)
{
	A(stateIndex, stateIndex) = H/dt;
	A(stateIndex, base->stateIndex) = -1.0;
	A(stateIndex, head->stateIndex) = 1.0;
	RHS(stateIndex) = oldCS.soln(base->stateIndex) - oldCS.soln(head->stateIndex);
}

Capacitor::Capacitor(Circuit& cir, double C, Node* n1, Node* n2, string name)
: TwoEndedDevice(n1, n2, name), C(C)
{
	cir.addDevice(this);
	n1->base.push_back(this);
	n2->head.push_back(this);
}

void Capacitor::writeEqn(const CircuitState oldCS, double dt, fullMatrix& A, fullMatrix& RHS)
{
	A(stateIndex, stateIndex) = 1.0;
	A(stateIndex, base->stateIndex) = -C/dt;
	A(stateIndex, head->stateIndex) = C/dt;
	RHS(stateIndex) = -oldCS.soln(stateIndex);
}

Resistor::Resistor(Circuit& cir, double R, Node* n1, Node* n2, string name)
: TwoEndedDevice(n1, n2, name), R(R)
{
	cir.addDevice(this);
	n1->base.push_back(this);
	n2->head.push_back(this);
}

void Resistor::writeEqn(const CircuitState oldCS, double dt, fullMatrix& A, fullMatrix& RHS)
{
	A(stateIndex, stateIndex) = R;
	A(stateIndex, head->stateIndex) = 1.0;
	A(stateIndex, base->stateIndex) = -1.0;
	RHS(stateIndex) = -R * oldCS.soln(stateIndex) + oldCS.soln(base->stateIndex) - oldCS.soln(head->stateIndex);
}

Diode::Diode(Circuit& cir, Node* n1, Node* n2, string name)
: TwoEndedDevice(n1, n2, name)
{
	cir.addDevice(this);
	n1->base.push_back(this);
	n2->head.push_back(this);
}

void Diode::writeEqn(const CircuitState oldCS, double dt, fullMatrix& A, fullMatrix& RHS)
{
	double Vc = .5, Rhigh = 1e6, Rlow = .01;
	double dV = oldCS.soln(base->stateIndex) - oldCS.soln(head->stateIndex);
	double I = (dV <= Vc) ? dV/Rhigh : (dV - Vc)/Rlow + Vc/Rhigh;

	if(dV < Vc){
		A(stateIndex, stateIndex) = 1.0;
		A(stateIndex, head->stateIndex) = 1/Rhigh;
		A(stateIndex, base->stateIndex) = -1/Rhigh;
		RHS(stateIndex) = dV/Rhigh - I;
	}
	else {
		A(stateIndex, stateIndex) = 1.0;
		A(stateIndex, head->stateIndex) = 1/Rlow;
		A(stateIndex, base->stateIndex) = -1/Rlow;
		RHS(stateIndex) = (dV - Vc)/Rlow + Vc/Rhigh - I;
	}
}

ExternalVoltage::ExternalVoltage(Circuit& cir, funct f, Node* n, string name)
: OneEndedDevice(n, name), f(f)
{
	cir.addDevice(this);
	n->base.push_back(this);
}

void ExternalVoltage::writeEqn(const CircuitState oldCS, double dt, fullMatrix& A, fullMatrix& RHS)
{
	A(stateIndex, base->stateIndex) = 1.0;
	RHS(stateIndex) = f(oldCS.time + dt) - oldCS.soln(base->stateIndex);
}

CircuitState::CircuitState(Circuit& circuit)
: soln(circuit.stateLen), cir(&circuit)
{
//	Circuit* cir;
//	cir = &circuit;
}

void CircuitState::zero()
{
	uint i;

	for(i=0; i < cir->stateLen; i++)
		soln(i) = 0;
	time = 0;
}

void CircuitStep(CircuitState& state, CircuitState& next, double dt)
{
	fullMatrix A(state.cir->stateLen, state.cir->stateLen);
	fullMatrix RHS(state.cir->stateLen);
	fullMatrix dstate(state.cir->stateLen);
	uint i;

	for(i=0; i < state.cir->Devices.size(); i++)
		state.cir->Devices[i]->writeEqn(state, dt, A, RHS);
	for(i=0; i < state.cir->Nodes.size(); i++)
		state.cir->Nodes[i]->writeEqn(state, dt, A, RHS);

	solve(A, dstate, RHS);
	next.soln = state.soln + dstate;
	next.time = state.time + dt;
}
