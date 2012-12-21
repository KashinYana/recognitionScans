CC              := g++
CFLAGS          := -I/usr/local/include/opencv2 -I/usr/local/include/opencv -std=c++0x 
OBJECTS         := 
LIBRARIES       := -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_objdetect -lopencv_ml

.PHONY: all clean

all: main

main:
	$(CC) $(CFLAGS) -O2 -o update.exe update.cpp $(LIBRARIES)

clean:
	rm -f *.o *.exe
