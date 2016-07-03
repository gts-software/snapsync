CC=gcc
CXX=g++

CFLAGS=-Wall -g
CXXFLAGS=-Wall -g -std=c++11

CXXINCLUDES=-Ilibrsync/src -I.
CXXLIBS=-lboost_system -lboost_filesystem -Llibrsync -lrsync -Lcryptopp -lcryptopp
CXXOBJECTS := $(patsubst src/%.c++,build/%.obj,$(filter-out %_main.c++,$(wildcard src/*.c++)))

all: build/snap build/sync

build/snap: $(CXXOBJECTS) build/snap_main.obj
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -o $@ $^ $(CXXLIBS)

build/sync: $(CXXOBJECTS) build/sync_main.obj
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -o $@ $^ $(CXXLIBS)

build/%.obj: src/%.c++
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -MD -MF $(patsubst %.obj,%.deps,$@) $(CXXINCLUDES) -c -o $@ $<

-include $(CXXOBJECTS:.obj=.deps) build/snap_main.deps build/sync_main.deps
