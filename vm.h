#ifndef clox_vm_h
#define clox_vm_h

#include "chunk.h"
#include "value.h" // actually it's not necessary. value.h has been included in chunk.h.
#define STACK_MAX 1024

typedef struct {
    Chunk* chunk;
    uint8_t* ip;
    Value stack[STACK_MAX];
    Value* stackTop;
} VM;

typedef enum {
  INTERPRET_OK,
  INTERPRET_COMPILE_ERROR,
  INTERPRET_RUNTIME_ERROR
} InterpretResult;

void initVM();
void freeVM();
InterpretResult interpret(const char* source);
void push(Value value);
Value pop();


#endif