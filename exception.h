//
//  exception.h
//  C-Demo
//
//  Created by Zhu Shengqi on 05/02/2017.
//  Copyright © 2017 Zhu Shengqi. All rights reserved.
//

#pragma once

#include <setjmp.h>

struct Exception {
    const char *reason;
};

struct Exception_Context {
    struct Exception_Context *prev;
    jmp_buf env;
    const char *file;
    int line;
    const struct Exception *exception;
};

enum Exception_Status {
    Exception_Entered = 0,
    Exception_Raised,
    Exception_Handled,
    Exception_Finalized
};

extern struct Exception_Context *exception_stack;

void except_raise(const struct Exception *e, const char *file, int line);

#define RAISE(e) except_raise(&(e), __FILE__, __LINE__)
#define RERAISE except_raise(exception_context.exception, exception_context.file, exception_context.line)
#define RETURN switch (exception_stack = exception_stack->prev, 0) default: return

#define TRY \
            { \
                volatile enum Exception_Status exception_status; \
                struct Exception_Context exception_context; \
                exception_context.prev = exception_stack; \
                exception_stack = &exception_context; \
                exception_status = setjmp(exception_context.env); \
                if (exception_status == Exception_Entered) {
#define CATCH(e) \
                    if (exception_status == Exception_Entered) exception_stack = exception_stack->prev; \
                } else if (exception_context.exception == &(e)) { \
                    exception_status = Exception_Handled;
#define ELSE \
                    if (exception_status == Exception_Entered) exception_stack = exception_stack->prev; \
                } else { \
                    exception_status = Exception_Handled;
#define FINALLY \
                    if (exception_status == Exception_Entered) exception_stack = exception_stack->prev; \
                } { \
                    if (exception_status == Exception_Entered) \
                        exception_status = Exception_Finalized;
#define END_TRY \
                    if (exception_status == Exception_Entered) exception_stack = exception_stack->prev; \
                } \
                if (exception_status == Exception_Raised) RERAISE; \
            }

