CC=gcc
CXX=g++

CFLAGS=-Wall -g
CXXFLAGS=-Wall -g -std=c++11

CXXINCLUDES=-Ilibrsync/src -Icryptopp
CXXLIBS=-lboost_system -lboost_filesystem -Llibrsync -lrsync -Lcryptopp -lcryptopp
CXXOBJECTS := $(patsubst src/%.c++,build/%.obj,$(wildcard src/*.c++))

build/snapsync: $(CXXOBJECTS)
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -o $@ $^ $(CXXLIBS)

build/%.obj: src/%.c++
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -MD -MF $(patsubst %.obj,%.deps,$@) $(CXXINCLUDES) -c -o $@ $<

-include $(CXXOBJECTS:.obj=.deps)
