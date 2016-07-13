CC=g++
CFLAGS=-lwiringPi
HFILES=shift166.h common.h radioArray.h logger.h
CPPFILES=main.cpp shift166.cpp common.cpp radioArray.cpp logger.cpp
EXE=radiometer
LOGFILE=radio.log
ARGS=$(LOGFILE)

$(EXE):$(CPPFILES) $(HFILES)
	 $(CC) -o $(EXE) $(CPPFILES) $(CFLAGS)

run: $(EXE)
	sudo ./$(EXE) $(ARGS)

clean:
	rm -fv $(EXE) *~ $(LOGFILE)
