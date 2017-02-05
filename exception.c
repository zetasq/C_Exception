//
//  exception.c
//  C-Demo
//
//  Created by Zhu Shengqi on 05/02/2017.
//  Copyright © 2017 Zhu Shengqi. All rights reserved.
//

#include "exception.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


struct Exception_Context *exception_stack = NULL;

void except_raise(const struct Exception *e, const char *file, int line) {
    struct Exception_Context *ctx_ptr = exception_stack;
    assert(e);
    
    if (!ctx_ptr) {
        fprintf(stderr, "Uncaught exception");
        
        if (e->reason) {
            fprintf(stderr, " %s", e->reason);
        } else {
            fprintf(stderr, " at 0x%p", e);
        }
        
        if (file && line > 0) {
            fprintf(stderr, " raised at %s:%d\n", file, line);
        }
        
        fprintf(stderr, "aborting...\n");
        fflush(stderr);
        abort();
    }
    
    ctx_ptr->exception = e;
    ctx_ptr->file = file;
    ctx_ptr->line = line;
    
    exception_stack = exception_stack->prev;
    longjmp(ctx_ptr->env, Exception_Raised);
}

