SRCDIR = src
INCDIR = include
OUTDIR = build
LIBDIR = lib
OBJDIR = bin
REMDIR = test/syncroot_rem
LOCDIR = test/syncroot_local

CC = gcc
CFLAGS = -I$(INCDIR) -g
LIBS =

_DEPS = utils.h comm.h macros.h config.h
DEPS = $(patsubst %, $(INCDIR)/%, $(_DEPS))

_OBJ_SERV = server.o utils.o comm.o
OBJ_SERV = $(patsubst %, $(OUTDIR)/%, $(_OBJ_SERV))

_OBJ_CLIENT = client.o utils.o comm.o
OBJ_CLIENT = $(patsubst %, $(OUTDIR)/%, $(_OBJ_CLIENT))

$(OUTDIR)/%.o: $(SRCDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

server: $(OBJ_SERV)
	$(CC) -o $(OBJDIR)/$@ $^ $(CFLAGS) $(LIBS)

client: $(OBJ_CLIENT)
	$(CC) -o $(OBJDIR)/$@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(OUTDIR)/*.o $(SRCDIR)/*~ $(INCDIR)/*~ $(OBJDIR)/* $(REMDIR)/*
