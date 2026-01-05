
PROGRAM_NAME = programa_1.c
EXE=$(shell basename $(PROGRAM_NAME) .c)


.PHONY: all run clean

all: run

run:
	@echo "--- Iniciando programa ---"
	@gcc $(PROGRAM_NAME) -o $(EXE)
	@./$(EXE)
	
clean:
	@rm $(EXE)
