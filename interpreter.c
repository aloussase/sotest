#include "interpreter.h"
#include <ffi.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

#include <string.h>

static void eval_load_expr(so_interpreter *, so_expr_load *load, so_expr *expr);

static void eval_call_expr(so_interpreter *, so_expr_call *call, so_expr *expr);

void so_interpreter_init(so_interpreter *interp) {
    memset(interp->library_handles, 0, sizeof(void *) * MAX_LOAD);
    memset(interp->library_names, 0, sizeof(char *) * MAX_LOAD);
    interp->loaded_libraries = 0;
}

void so_interpreter_deinit(so_interpreter *interp) {
    for (int i = 0; i < interp->loaded_libraries; i++) {
        dlclose(interp->library_handles[i]);
        free(interp->library_names[i]);
    }
}

void so_interpreter_run(so_interpreter *interp, const char *source) {
    so_parser parser;
    so_parser_init(&parser, source);
    so_parser_parse(&parser);

    for (int i = 0; i < parser.ncommands; i++) {
        so_expr *command = parser.commands[i];
        switch (command->tag) {
            case SO_EXPR_LOAD:
                eval_load_expr(interp, &command->as.load, command);
                break;
            case SO_EXPR_CALL:
                eval_call_expr(interp, &command->as.call, command);
                break;
            default: break;
        }

        free_ast(command);
    }

    so_parser_deinit(&parser);
}


void eval_load_expr(so_interpreter *interp, so_expr_load *load, so_expr *expr) {
    void *handle = dlopen(load->name, RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "error: failed to load dynamic library: %s\n", load->name);
        free_ast(expr);
        so_interpreter_deinit(interp);
        exit(EXIT_FAILURE);
    }

    interp->library_handles[interp->loaded_libraries] = handle;
    interp->library_names[interp->loaded_libraries] = strdup(load->name);
    interp->loaded_libraries++;
}

void eval_call_expr(so_interpreter *interp, so_expr_call *call, so_expr *expr) {
    void *function_handle = NULL;

    for (int i = 0; i < interp->loaded_libraries; i++) {
        function_handle = dlsym(interp->library_handles[i], call->name);
        if (function_handle)
            break;
    }

    if (!function_handle) {
        fprintf(stderr, "error: could not find the function: %s\n", call->name);
        free_ast(expr);
        so_interpreter_deinit(interp);
        exit(EXIT_FAILURE);
    }

    ffi_cif cif;

    ffi_type **argument_types = malloc(sizeof(ffi_type *) * call->nargs);
    void **argument_values = malloc(sizeof(void *) * call->nargs);

    for (int i = 0; i < call->nargs; i++) {
        if (call->args[i]->tag == SO_EXPR_INT) {
            argument_types[i] = &ffi_type_sint;
            argument_values[i] = &call->args[i]->as.number;
        }

        if (call->args[i]->tag == SO_EXPR_STRING) {
            argument_types[i] = &ffi_type_pointer;
            argument_values[i] = &call->args[i]->as.string;
        }
    }

    ffi_status status = ffi_prep_cif(
        &cif,
        FFI_DEFAULT_ABI,
        call->nargs,
        &ffi_type_void,
        argument_types);

    if (status != FFI_OK) {
        fprintf(stderr, "error: failed to prepare ffi call: %s\n", call->name);
        free_ast(expr);
        so_interpreter_deinit(interp);
        free(argument_types);
        free(argument_values);
        exit(EXIT_FAILURE);
    }

    // At this point is to the caller to have provided the right arguments
    ffi_call(&cif, function_handle, NULL, argument_values);

    free(argument_types);
    free(argument_values);
}
