CXX = g++
CXXFLAGS += -std=c++11 -Wall -g
CPPFLAGS += `pkg-config --cflags opencv` -I../ 
LDFLAGS  += `pkg-config --libs   opencv` libSimd.a \
            -lpthread -lz -lm -lrt -ldl

SRC += MyTest.cpp MyLog.cpp

OBJS += $(patsubst %.cpp, %.o, $(SRC))

all: MyTest

%.o: %.cpp
	@echo ">>> Compile $@ begin >>>"
	$(CXX) -c -o $(@) $(CXXFLAGS) $(CPPFLAGS) $<
	@echo "<<< Compile $@ end <<<\n"

MyTest: $(OBJS)
	# @echo ">>> Link $(OBJS) to main begin >>>"
	$(CXX) -o $(@) $(CXXFLAGS) $(OBJS) $(LDFLAGS)
	# @echo "<<< Link $(OBJS) to main end <<<"

clean:
	@echo "*** Removing linked and compiled files ***"
	rm -f $(OBJS)
	rm -f MyTest


