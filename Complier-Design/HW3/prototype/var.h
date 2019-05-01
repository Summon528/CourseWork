typedef struct Var_t {
    char *name;
    int *arr;
    int arr_capacity;
    int arr_cnt;
} Var_t;

void addArr(Var_t *v, int x);
