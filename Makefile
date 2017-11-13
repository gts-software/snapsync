CC=gcc
CXX=g++

CFLAGS=-Wall -g
CXXFLAGS=-Wall -g

CXXINCLUDES=-Ilibrsync/src -I.
CXXLIBS=-lboost_system -lboost_filesystem -Llibrsync -lrsync -Lcryptopp -lcryptopp
CXXOBJECTS := $(patsubst src/%.cc,build/%.obj,$(filter-out %_main.cc,$(wildcard src/*.cc)))

all: build/justsnap build/justsync

build/justsnap: $(CXXOBJECTS) build/snap_main.obj
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -o $@ $^ $(CXXLIBS)

build/justsync: $(CXXOBJECTS) build/sync_main.obj
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -o $@ $^ $(CXXLIBS)

build/%.obj: src/%.cc Makefile
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -MD -MF $(patsubst %.obj,%.deps,$@) $(CXXINCLUDES) -c -o $@ $<

-include $(CXXOBJECTS:.obj=.deps) build/snap_main.deps build/sync_main.deps
