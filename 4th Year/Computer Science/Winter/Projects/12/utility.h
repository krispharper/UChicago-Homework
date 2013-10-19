// utility.h 
#ifndef UTILITY_H
#define UTILITY_H


#include <cstdlib>
#include <iostream>

// write message and die
void error( const char s[] );

// write message but keep cranking
void warning( const char s[], std::ostream& out );

#endif
