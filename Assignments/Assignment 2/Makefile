CC = gcc
CFLAGS = -Wall
SOURCES = u2021562_main.c utilities.c u2021562_fifo.c u2021562_sjf.c u2021562_stcf.c u2021562_rr.c
EXECUTABLE = run

.PHONY: clean rebuild runfifo runsjf runstcf runrr

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(EXECUTABLE)

clean:
	rm -f $(EXECUTABLE)

rebuild: clean all

runfifo: $(EXECUTABLE)
	./$(EXECUTABLE) processes.dat FIFO

runsjf: $(EXECUTABLE)
	./$(EXECUTABLE) processes.dat SJF

runstcf: $(EXECUTABLE)
	./$(EXECUTABLE) processes.dat STCF

runrr: $(EXECUTABLE)
	./$(EXECUTABLE) processes.dat RR
