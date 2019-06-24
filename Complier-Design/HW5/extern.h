#include "everything.h"

TableStack_t* ts;
int linenum;
int Opt_Symbol;
Type_t cur_type, cur_fun_type;
int in_loop;
bool r_state;
bool has_semantic_error;
FILE* codeout;
bool in_main_fun;
int label_cnt;
IntArray_t *exit_labels, *continue_labels;
