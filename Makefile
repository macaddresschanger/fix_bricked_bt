CXX=g++
CPPFLAGS=-g
CXXFLAGS=-Wall
LDFLAGS=-g
LDLIBS=-lusb-1.0

SRCS=main.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all: fix_bricked_bt

fix_bricked_bt: $(OBJS)
	$(CXX) $(LDFLAGS) -o fix_bricked_bt $(OBJS) $(LDLIBS)

clean:
	rm $(OBJS) fix_bricked_bt
