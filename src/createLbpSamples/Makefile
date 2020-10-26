CXX = g++
CXXFLAGS += -std=c++11 -Wall -g
CPPFLAGS += `pkg-config --cflags opencv hdf5 glib-2.0` -I../
LDFLAGS  += `pkg-config --libs   opencv hdf5 glib-2.0` \
	    ../../prj/cmake/libSimd.a \
            -lpthread -lz -lm -lrt -ldl

SRC += createLbpSamples.cpp utility.cpp \
       ../lbpSrc/LbpHistogram.cpp ../lbpSrc/LbpViewLogger.cpp ../lbpSrc/StrLogger.cpp

OBJS += $(patsubst %.cpp, %.o, $(SRC))

all: createLbpSamples

%.o: %.cpp
	@echo ">>> Compile $@ begin >>>"
	$(CXX) -c -o $(@) $(CXXFLAGS) $(CPPFLAGS) $<
	@echo "<<< Compile $@ end <<<\n"

createLbpSamples: $(OBJS)
	# @echo ">>> Link $(OBJS) to main begin >>>"
	$(CXX) -o $(@) $(CXXFLAGS) $(OBJS) $(LDFLAGS)
	# @echo "<<< Link $(OBJS) to main end <<<"

clean:
	@echo "*** Removing linked and compiled files ***"
	rm -f $(OBJS)
	rm -f createLbpSamples


