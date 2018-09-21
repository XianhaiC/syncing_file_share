SDIR = src
IDIR = include
ODIR = build
LDIR = lib
BDIR = bin

CC = gcc
CFLAGS = -I$(IDIR)
LIBS =

_DEPS = utils.h comm.h macros.h config.h
DEPS = $(patsubst %, $(IDIR)/%, $(_DEPS))

_OBJ_SERV = server.o utils.o comm.o
OBJ_SERV = $(patsubst %, $(ODIR)/%, $(_OBJ_SERV))

_OBJ_CLIENT = client.o utils.o comm.o
OBJ_CLIENT = $(patsubst %, $(ODIR)/%, $(_OBJ_CLIENT))

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

server: $(OBJ_SERV)
	$(CC) -o $(BDIR)/$@ $^ $(CFLAGS) $(LIBS)

client: $(OBJ_CLIENT)
	$(CC) -o $(BDIR)/$@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(SDIR)/*~ $(IDIR)/*~ $(BDIR)/*
