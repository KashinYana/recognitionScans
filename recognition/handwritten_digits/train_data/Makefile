CC              := g++
CFLAGS          := -I/usr/local/include/opencv2 -I/usr/local/include/opencv -std=c++0x -g
OBJECTS         := 
LIBRARIES       := -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_objdetect -lopencv_ml

.PHONY: all clean

all: write show read

read:
	$(CC) $(CFLAGS) -O2 -o read.exe read.cpp $(LIBRARIES)

write:
	$(CC) $(CFLAGS) -O2 -o write.exe write.cpp $(LIBRARIES)

show:
	$(CC) $(CFLAGS) -O2 -o show.exe show.cpp $(LIBRARIES)

update:
	$(CC) $(CFLAGS) -O2 -o update.exe update.cpp $(LIBRARIES)

clean:
	rm -f *.o *.exe
