CC              := g++
CFLAGS          := -I/usr/local/include/opencv2 -I/usr/local/include/opencv -std=c++0x
OBJECTS         := 
LIBRARIES       := -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_objdetect

.PHONY: all clean

all: test_anchor_finder

test_anchor_finder: 
	$(CC) $(CFLAGS) -o anchor_finder_test.exe anchor_finder_test.cpp -g $(LIBRARIES)

clean:
	rm -f *.o *.exe
