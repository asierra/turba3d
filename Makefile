CC = g++
CFLAGS = -c -Wall -g
PROG = turba3d

SRCS = main.cpp crowdsRVO2.cpp blocks.cpp basic3d.cpp
#SRCS = cubemaps_skybox.cpp
OBJECTS=$(SRCS:.cpp=.o)
HEADERS=$(SRCS:.cpp=.h)
INCLUDES = -I./include

ifeq ($(shell uname),Darwin)
	LIBS = -framework Cocoa -framework OpenGL -lglfw -framework IOKit
else
	LIBS = -lglfw -lGL -ldl -L./lib -lglad -lRVO -lSTB_IMAGE
endif

all: $(PROG)

$(PROG):	$(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) $(LIBS) -o $@

.cpp.o: $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@

clean:
	rm -f $(PROG) *.o *~

