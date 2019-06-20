#pragma once
#define INITIAL_CAPACITY 4
#define MAX_LOCAL 100
typedef enum type {
    _int,
    _float,
    _double,
    _bool,
    _string,
    _void,
    _function,
    _unknown
} Type_t;
typedef enum kind {
    function,
    parameter,
    variable,
    constant,
    const_as_var
} Kind_t;
