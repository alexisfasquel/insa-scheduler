#include "process.h"

void
init_ctx(struct ctx_s* ctx, func_t f, unsigned int stack_size)
{
   ctx->sp = AllocateMemory(stack_size) + stack_size - 1;
   ctx->pc = (uint32_t)f; /* ? */
}

