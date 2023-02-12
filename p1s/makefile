COMPILER= gcc
OPTIONS = -g
LDFLAGS  = -lreadline
COMPILE = $(COMPILER) $(OPTIONS) 
BUILD = build
PROGRAM = PMan.c
PMan:  PMan.c
	$(COMPILER) PMan.c $(LDFLAGS) -g -o PMan

run: PMan
	./PMan

clean: 
	rm -f PMan

