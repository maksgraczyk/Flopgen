CXXFLAGS = --std=c++17

CC = $(CXX)
CCFLAGS = $(CXXFLAGS)

flopgen: fatfs/diskio.o fatfs/fattime.o fatfs/ff.o fatfs/ffsystem.o fatfs/ffunicode.o classes.o image.o filediskio.o main.o
	$(CXX) -o $@ $^

fatfs/%.o: fatfs/%.c fatfs/%.h
classes.o: classes.cpp classes.hpp image.hpp
image.o: image.cpp image.hpp classes.hpp fatfs/ff.h filediskio.h
filediskio.o: filediskio.cpp filediskio.h fatfs/ff.h fatfs/diskio.h classes.hpp
main.o: main.cpp classes.hpp cli/CLI11.hpp

.PHONY: clean

clean:
	rm -f *.o fatfs/*.o flopgen flopgen.exe
