#include "checksum.h"

// retreive the checksum of a file
int checksum_file(char *path, char *checksum) {
    int br;
    char buf[BUF_LEN];
    FILE *fp;

    SHA_CTX ctx;

    fp = fopen(path, "r");

    if (fp == NULL) {
        perror("fopen");
        return -1;
    }

    SHA1_Init(&ctx);

    // begin sending the file
    while (1) {
        // buffer in a chunk of file data
        br = fread(buf, sizeof(char), BUF_LEN, fp);

        // error handling
        if (ferror(fp)) {
            fclose(fp);
            return -1;
        }

        // update checksum with buffered bytes
        SHA1_Update(&ctx, buf, br);

        // break if we are at EOF
        if (feof(fp)) {
            break;
        }
    }

    // return the final checksum
    SHA1_Final(checksum, &ctx);

    fclose(fp);

    // success
    return 0;
}

// validate the local checksum with the correct origin checksum
int checksum_validate(char *checksum_origin, char *checksum_local) {
    int i;

    for (i = 0; i < SHA_DIGEST_LENGTH; i++) {
        // if checksum char differs then return false
        if (checksum_origin[i] != checksum_local[i]) return 0;
    }

    // checksums are the same
    return 1;
}
