# what flags you want to pass to the C compiler & linker
AM_CXXFLAGS = --pedantic -Wall -O2 -std=gnu++0x
#LDFLAGS =
# this lists the binaries to produce, the (non-PHONY, binary) targets in
# the previous manual Makefile
#bin_PROGRAMS = 

lib_LIBRARIES = libRealTime.a
libRealTime_a_SOURCES =    \
        RealTimeTask.cpp   \
        RealTimeTask.h     \
        RT_TaskManager.cpp \
        RT_TaskManager.h   \
        RTT_Interface.h

include_HEADERS = \
        RealTimeTask.h   \
        RT_TaskManager.h \
        RTT_Interface.h





