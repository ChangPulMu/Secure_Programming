
IDIR = ../include
CC=gcc
CFLAGS=-I$(IDIR)

ODIR = obj
LDIR = ../lib

LIBS = -Im

_DEPS = module.h
DEPS = $(patsubst %, $(IDIR)/%, $(_DEPS))

_OBJ = main.o module.o
OBJ = $(patsubst %, $(ODIR)/%, $(_OBJ))

$(ODIR)/%.o : %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main.out : $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY : clean
clean :
	rm -f main.out $(ODIR)/*.o *~ core $(IDIR)/*~
