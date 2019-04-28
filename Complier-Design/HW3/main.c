#include <stdlib.h>
#include "decl_list.h"
#include "symbol_table.h"
#include "table_stack.h"

int main() {
    TableStack_t* ts = newTableStack();
    // SymbolTable_t* st = newSymbolTable();
    // DeclList_t* dl = newDeclList();
    // addDeclEntry(dl, "abc", "123");
    // addDeclEntry(dl, "abc", "45123");
    // addDeclEntry(dl, "ac", "4512443");
    // addDeclEntry(dl, "zzz", "77512443");
    // addDeclEntry(dl, "fff", "-5");
    // addConstFromList(st, dl, _int);
    pushTable(ts);
    DeclList_t* dl = newDeclList();
    addDeclEntry(dl, "abc", NULL, NULL);
    addDeclEntry(dl, "abc", NULL, "10");
    addDeclEntry(dl, "ac", NULL, "30");
    addDeclEntry(dl, "zzz", NULL, NULL);
    addDeclEntry(dl, "fff", NULL, "8");
    addVarFromList(getTopTable(ts), dl, _int);

    // addConstEntry(st, "abc", "123", _int);
    // addConstEntry(st, "cfg", "6866123", _int);
    // addConstEntry(st, "regex", "68", _int);
    // addConstEntry(st, "fl", "68.3", _float);
    // addConstEntry(st, "sci", "1e3", _double);
    // addConstEntry(st, "d", "1.3", _double);
    // addConstEntry(st, "b", "1.3", _bool);
    // addConstEntry(st, "hahas", "tsengcy", _string);
    printTable(getTopTable(ts));
    popTable(ts);
}
