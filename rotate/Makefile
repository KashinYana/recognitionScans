CC              := g++
CFLAGS          := -I/usr/local/include/opencv2 -I/usr/local/include/opencv -std=c++0x 
OBJECTS         := 
LIBRARIES       := -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_objdetect

.PHONY: all clean

all: anchor_finder_test anchor_rotater_test

anchor_finder_test: 
	$(CC) $(CFLAGS) -o anchor_finder_test.exe -g anchor_finder_test.cpp $(LIBRARIES)

anchor_rotater_test:
	$(CC) $(CFLAGS) -o anchor_rotater_test.exe -g anchor_rotater_test.cpp $(LIBRARIES)

clean:
	rm -f *.o *.exe
