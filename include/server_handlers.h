#ifndef SERVER_HANDLERS_H
#define SERVER_HANDLERS_H

typedef struct _context context;

struct _context {
    ht_file *inode_res;
    ht_file *clientid_res;
};

#endif
