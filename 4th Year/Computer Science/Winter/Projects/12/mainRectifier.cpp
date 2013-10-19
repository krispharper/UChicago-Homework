// rectifier and filter only

#include "devices.h"
#include <cmath>
#include <fstream>

double zero( double time );
double ac( double time );


double zero( double time )
{ // ground
  return 0.0; 
}

double ac( double time )
{ // 10volts AC
return 10*std::sin( 2*3.141592653589*60*time );
}

int main()
{// simulation of a rectifier and filter

  std::ofstream outR20dv("R02_00_dV");
  std::ofstream outN3("Node3_V");

  // set up the circuit
  Circuit cir;

  Node n00(cir, "node00");
  Node n01(cir, "node01");
  Node n02(cir, "node02");
  Node n03(cir, "node02");
  Node n04(cir, "node02");

  Inductor L01_02(cir, 0.02, &n01, &n02, "InductorL01_02" );
  Capacitor C02_00(cir, 0.002, &n00, &n02, "CapacitorC02_00");
  Capacitor C01_00(cir, 0.002, &n00, &n01, "CapacitorC00_01");
  Resistor R02_00(cir, 10.0, &n00, &n02, "ResistorR02_00" );
  Diode D03_01(cir, &n03, &n01, "Diode03_01");
  Diode D00_03(cir, &n00, &n03, "Diode00_03");
  Diode D00_04(cir, &n00, &n04, "Diode00_04");
  Diode D04_01(cir, &n04, &n01, "Diode04_01");
  ExternalVoltage acEV( cir, ac, &n03, "ACsource" );
  ExternalVoltage grnd( cir, zero, &n04, "Ground" );

  // define two states and intialize one to zero
  CircuitState current(cir), next(cir);
  current.zero();

  // time step specification
  int spc = 40; // steps per cycle
  double dt = 1./(60.0*spc);
  int numbSteps = 8*spc;

  for( int step = 0; step < numbSteps; ++step ){
    // simulate one step
    CircuitStep( current, next, dt );   
    //write out some data
    outR20dv << current.time << '\t';
    outR20dv << -R02_00.R*current.soln(R02_00.stateIndex)
             << std::endl;
    outN3 << current.time << '\t'
          << current.soln(n03.stateIndex) << std::endl;
    current = next;
  }

  // write last values out too
    outR20dv << current.time << '\t';
    outR20dv << -R02_00.R*current.soln(R02_00.stateIndex)
             << std::endl;
    outN3 << current.time << '\t'
          << current.soln(n03.stateIndex) << std::endl;

  return 0;
}



