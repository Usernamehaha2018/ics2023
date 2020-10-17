#include "../local-include/rtl.h"

#include "arith.h"
#include "control.h"
#include "data-mov.h"
#include "logic.h"
#include "string.h"
#include "system.h"

def_EHelper(nop);
def_EHelper(inv);
def_EHelper(nemu_trap);
def_EHelper(call);
def_EHelper(push);
def_EHelper(pop);
def_EHelper(xor);
