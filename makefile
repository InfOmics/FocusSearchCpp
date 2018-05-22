CC=g++
CFLAGS=-O3 -w -c -Wall
LDFLAGS=-w -lstdc++ 
SOURCES= IsoFocus.cpp src/gLists.cpp src/gdex_predicate.cpp src/gpreproc.cpp  \
	src/gsequence.cpp src/fsearch.cpp src/iso_check.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=fosearch
INCLUDES=-I./include

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< $(INCLUDES) -o $@

clean:
	rm fosearch
	rm IsoFocus.o
	rm src/*.o
