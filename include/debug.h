#pragma once
#include <stdio.h>
#include <stdlib.h>

enum FL_ERROR
{
    FL_NOTICE = 0,
    FL_FATAL = 1,
    FL_WARNING = 2,
};


void fl_error(const char* msg, enum FL_ERROR type);