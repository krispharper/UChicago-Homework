// rectifier and filter only

#include "devices.h"
#include <cmath>
#include <fstream>

double zero( double time );
double ten( double time );


double zero( double time )
{ // ground
  return 0.0; 
}

double ten( double time )
{ // 10volts DC
  return 10.0;
}

int main()
{// simulation of a rectifier and filter

  std::ofstream outR20dv("R02_00_dV");

  // set up the circuit
  Circuit cir;

  Node n00(cir, "node00");
  Node n02(cir, "node02");


  Resistor R02_00(cir, 20.0, &n00, &n02, "ResistorR02_00" );
  ExternalVoltage dcEV( cir, ten, &n00, "DCsource" );
  ExternalVoltage grnd( cir, zero, &n02, "Ground" );

  // define two states and intialize one to zero
  CircuitState current(cir), next(cir);
  current.zero();

  // time step specification
  double dt = 1./30.0;
    int numbSteps = 10;

  for( int step = 0; step < numbSteps; ++step ){
    // simulate one step
    CircuitStep( current, next, dt );   
    //write out some data
    outR20dv << current.time << '\t';
    outR20dv << current.soln(R02_00.stateIndex)
             << std::endl;
    current = next;
  }

  // write last values out too
    outR20dv << current.time << '\t';
    outR20dv << current.soln(R02_00.stateIndex)
             << std::endl;

  return 0;
}



