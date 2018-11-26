/*
 * functions that handle general sync utility
 */

// provides the path name for the duplicate of the given file path
void su_get_dup_path(char *path_ori, char *path_dup, uuit_t id) {
    // create path_dup with format:
    // <path>.dup~<id>
    strcpy(path_dup, path_ori);
    strcat(path_dup, DUP_EXT);
    strncat(path_dup, id, sizeof(uuid_t));
}
