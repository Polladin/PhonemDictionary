
SOURCES += Dictionary.cpp
SOURCES += DictionaryStringParser.cpp
SOURCES += PhonemNode.cpp
SOURCES += PrefixTreeCompressor.cpp
SOURCES += PrefixTree.cpp
SOURCES += main.cpp

CC = g++
LD = g++

# Toolchain arguments.
CFLAGS = -O3 -std=c++1y -c
LFLAGS = -O3 

# Project sources
OBJECTS = $(SOURCES:.cpp=.o)

# The dependency file names
DEPS = $(OBJECTS:.o=.d)

EXECUTABLE = PhonemDictionary


OBJ: $(EXECUTABLE)


clean:
	rm -rf *.o


$(EXECUTABLE): $(OBJECTS)
	$(LD) $(LFLAGS) $(OBJECTS) $(INCLUDES) -o $@


.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@


-include $(DEPS)

