SUBDIRS		= 
OBJECTS_C	= $(shell find $$pwd -name '*.cpp')
OBJECTS = $(OBJECTS_C:%.cpp=%.o)

all: subdirs ${OBJECTS}

clean:cleansubdirs
	rm -f ${OBJECTS}

include ${MAKEINCLUDE}
