#include "test.h"
#include "list.h"

int main() {
    int i;
    list list_test; 
    char * cp;
    int char_len;
    char * arr_string[] = {
        "hello",
        "world",
        "russia",
        "china",
        "bangladesh",
        "columbia",
        "denmark",
    };
    int arr_string_len = sizeof(arr_string) / sizeof(arr_string[0]);

    list_init(&list_test, &data_free_string);

    for (i = 0; i < arr_string_len; i++) {
        char_len = strlen(arr_string[i]);
        cp = (char *) calloc(char_len + 1, sizeof(char));
        strncpy(cp, arr_string[i], char_len);

        list_append(&list_test, (void *) cp);
    }

    // test size, capacity, and data contents
    for (i = 0; i < arr_string_len; i++) {
        TEST(strcmp(arr_string[i], list_get(&list_test, i)) == 0);
    }
    
    TEST(list_test.size == arr_string_len);
    TEST(list_test.capacity == LIST_INIT_LEN);

    for (i = 0; i < arr_string_len; i++) {
        list_remove(&list_test, i);
    }

    // test that the pointers in the list are all null
    for (i = 0; i < arr_string_len; i++) {
        TEST(list_test.data[i] == NULL);
    }

    list_free(&list_test);
}
