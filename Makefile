CC=g++
CFLAGS=-lwiringPi
HFILES=shift166.h common.h radioArray.h
CPPFILES=main.cpp shift166.cpp common.cpp radioArray.cpp
EXE=radiometer

$(EXE):$(CPPFILES) $(HFILES)
	 $(CC) -o $(EXE) $(CPPFILES) $(CFLAGS)

run: $(EXE)
	sudo ./$(EXE)

clean:
	rm -fv $(EXE) *~
