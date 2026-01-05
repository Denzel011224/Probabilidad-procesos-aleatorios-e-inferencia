
PROGRAM_NAME = programa_3.c
EXE=$(shell basename $(PROGRAM_NAME) .c)


.PHONY: all run clean

all: run

run:
	@echo "--- Iniciando programa ---"
	@gcc $(PROGRAM_NAME) -o $(EXE) -lm -lcsfml-graphics \
		 -lcsfml-window -lcsfml-system
	@./$(EXE)
	
clean:
	@rm $(EXE)
