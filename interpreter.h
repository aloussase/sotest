#ifndef interpreter_h_
#define interpreter_h_


#define MAX_LOAD 100

typedef struct {
    int loaded_libraries;
    char *library_names[MAX_LOAD];
    void *library_handles[MAX_LOAD];
} so_interpreter;

void so_interpreter_init(so_interpreter *);

void so_interpreter_deinit(so_interpreter *);

void so_interpreter_run(so_interpreter *, const char *source);

#endif
