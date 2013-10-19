LIB = -lm
CPP = g++
CPPFLAGS = -g

OBJS = \
devices.o\
mainRectifier.o\
matrix.o\
utility.o


powersupply : $(OBJS)
	$(CPP) $(CPPFLAGS) -o $@ $(OBJS) $(LIB)

devices.o: devices.cpp devices.h utility.h
	$(CPP) -c $(CPPFLAGS) devices.cpp
utility.o: utility.cpp utility.h
	$(CPP) -c $(CPPFLAGS) utility.cpp
mainRectifier.o: mainRectifier.cpp  devices.h
	$(CPP) -c $(CPPFLAGS) mainRectifier.cpp
matrix.o: matrix.cpp  matrix.h
	$(CPP) -c $(CPPFLAGS) matrix.cpp

results: powersupply
	powersupply

spotless : 
	rm powersupply *~  *.o *.ps *_dV *_V
