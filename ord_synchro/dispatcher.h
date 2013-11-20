#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <stdlib.h>
#include "process.h"

void  __attribute__((naked)) ctx_switch();//Change de contexte après une interruption
void  __attribute__((naked)) yield(struct pcb_s* pcb_next);//Change de contexte après une interruption

#endif
