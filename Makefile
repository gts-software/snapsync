CC=gcc
CXX=g++

CFLAGS=-Wall -g
CXXFLAGS=-Wall -g -std=c++11

CXXLIBS=-lboost_system -lboost_filesystem
CXXOBJECTS := $(patsubst src/%.c++,build/%.obj,$(wildcard src/*.c++))

build/snapdiff: $(CXXOBJECTS)
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -o $@ $^ $(CXXLIBS)

build/%.obj: src/%.c++
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -MD -MF $(patsubst %.obj,%.deps,$@) -c -o $@ $<

-include $(CXXOBJECTS:.obj=.deps)
