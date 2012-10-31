CC              := g++
CFLAGS          := #-I/usr/include/opencv2 
OBJECTS         := 
LIBRARIES       := -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_objdetect

.PHONY: all clean

all: test

test: 
	$(CC) $(CFLAGS) -o open open.cpp $(LIBRARIES)

create:
	$(CC) $(CFLAGS) -o create_xml create_xml.cpp $(LIBRARIES) 

white:
	$(CC) $(CFLAGS) -o white_black white_black.cpp $(LIBRARIES)     
hold:
	$(CC) $(CFLAGS) -o usingTresholding usingTresholding.cpp $(LIBRARIES)         
clean:
	rm -f *.o
