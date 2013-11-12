#ifndef __PROCESS_H
#define __PROCESS_H

#include "allocateMemory.h"

typedef void (*func_t) ( void);

struct ctx_s {
	uint32_t pc;
	uint32_t* sp;
	uint32_t r0;
	uint32_t r1;
	uint32_t r2;
	uint32_t r3;
	uint32_t r4;
	uint32_t r5;
	uint32_t r6;
	uint32_t r7;
	uint32_t r8;
	uint32_t r9;
	uint32_t r10;
	uint32_t r11;
	uint32_t r12;
	
};


struct ctx_s* current_ctx;

void init_ctx(struct ctx_s* ctx, func_t f, unsigned int stack_size);

#endif

