SRCDIR = src
INCDIR = include
OUTDIR = bin
LIBDIR = lib
OBJDIR = build
REMDIR = test/syncroot_rem
LOCDIR = test/syncroot_local
TESTDIR = test

OSSLDIR = /usr/include/openssl

CC = gcc
LIBS = -lssl -lcrypto -luuid
CFLAGS = -I$(INCDIR) $(LIBS) -g

_DEPS = macros.h config.h utils.h comm.h tracker.h list.h hash_map.h tree_file.h tf_node.h cmd.h sync.h
DEPS = $(patsubst %, $(INCDIR)/%, $(_DEPS))

_OBJ = utils.o comm.o tracker.o list.o hash_map.o tree_file.o tf_node.o cmd.o sync.o
OBJ = $(patsubst %, $(OBJDIR)/%, $(_OBJ))

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(OBJDIR)/*.o $(SRCDIR)/*~ $(INCDIR)/*~ $(OUTDIR)/* $(REMDIR)/* $(TOBJDIR)/*.o $(TESTDIR)/*~ $(TINCDIR)/*~ $(TOUTDIR)/* 

server: $(OBJDIR)/server.o $(OBJ)
	$(CC) -o $(OUTDIR)/$@ $^ $(CFLAGS) $(LIBS)

client: $(OBJDIR)/client.o $(OBJ)
	$(CC) -o $(OUTDIR)/$@ $^ $(CFLAGS) $(LIBS)


# unit test commands

TINCDIR = $(TESTDIR)/include
TOBJDIR = $(TESTDIR)/build
TOUTDIR = $(TESTDIR)/bin

CFLAGS_TEST = -I$(INCDIR) -I$(TINCDIR) $(LIBS) -g

_DEPS_TEST = test.h
DEPS_TEST = $(patsubst %, $(TINCDIR)/%, $(_DEPS_TEST))

$(TOBJDIR)/%.o: $(TESTDIR)/%.c $(DEPS) $(DEPS_TEST)
	@echo $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS_TEST)

t_tracker_load_changelog: $(TOBJDIR)/t_tracker_load_changelog.o $(OBJ)
	$(CC) -o $(TOUTDIR)/$@ $^ $(CFLAGS_TEST) 

t_tracker_save_changelog: $(TOBJDIR)/t_tracker_save_changelog.o $(OBJ)
	$(CC) -o $(TOUTDIR)/$@ $^ $(CFLAGS_TEST) 

t_list: $(TOBJDIR)/t_list.o $(OBJ)
	$(CC) -o $(TOUTDIR)/$@ $^ $(CFLAGS_TEST)

t_ht_file: $(TOBJDIR)/t_ht_file.o $(OBJ)
	$(CC) -o $(TOUTDIR)/$@ $^ $(CFLAGS_TEST)

t_ht_saveload: $(TOBJDIR)/t_ht_saveload.o $(OBJ)
	$(CC) -o $(TOUTDIR)/$@ $^ $(CFLAGS_TEST)

t_tree_file: $(TOBJDIR)/t_tree_file.o $(OBJ)
	$(CC) -o $(TOUTDIR)/$@ $^ $(CFLAGS_TEST)

t_tf_saveload: $(TOBJDIR)/t_tf_saveload.o $(OBJ)
	$(CC) -o $(TOUTDIR)/$@ $^ $(CFLAGS_TEST)
