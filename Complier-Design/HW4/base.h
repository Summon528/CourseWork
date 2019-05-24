#pragma once
#define INITIAL_CAPACITY 4
typedef enum type {
    _int,
    _float,
    _double,
    _bool,
    _string,
    _void,
    _array,
    _function,
    _unknown
} Type_t;
typedef enum kind { function, parameter, variable, constant } Kind_t;
