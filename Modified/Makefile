include Makefile.inc

DIRS	= protocols sim
EXE	= sim_trace
OBJS	= 
OBJLIBS	= lib/libprotocols.a lib/libsim.a 
LIBS	= -Llib/ -lsim -lprotocols
CXXFLAGS = -std=c++11 -Wall -g

all : $(EXE)

$(EXE) : $(OBJLIBS)
	g++ -std=c++11 -Wall -g -o sim_trace -pthread -Llib/ -lsim -lprotocols


lib/libprotocols.a : force_look
	cd protocols; $(MAKE) $(MFLAGS)

lib/libsim.a : force_look
	cd sim; $(MAKE) $(MFLAGS)

clean :
	$(ECHO) cleaning up in .
	-$(RM) -f $(EXE) $(OBJS) $(OBJLIBS)
	-for d in $(DIRS); do (cd $$d; $(MAKE) clean ); done

force_look :
	true
