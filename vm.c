#include <stdio.h>

#include "common.h"
#include "compiler.h"
#include "debug.h"
#include "vm.h"

VM vm; 

static void resetStack() {
  vm.stackTop = vm.stack;
}

void initVM() {
    resetStack();
}

void freeVM() {
}

static InterpretResult run() {
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define BINARY_OP(op)                                                   \
    do {                                                                \
      double b = pop();                                                 \
      double a = pop();                                                 \
      push(a op b);                                                     \
    } while (false)

    for (;;) {
        #ifdef DEBUG_TRACE_EXECUTION
            printf("          Current Stack: ");
            int _i = 0;
            for (Value* slot = vm.stack; slot < vm.stackTop; slot++) {
                _i++;
                printf("[ ");
                printValue(*slot);
                printf(" ]");
            }
            if(_i==0) printf(" Empty");
            printf("\n");
            disAssembleInstruction(vm.chunk,
                                (int)(vm.ip - vm.chunk->code));
        #endif
        uint8_t instruction;
        switch (instruction = READ_BYTE()) {
            case OP_CONSTANT: {
                Value constant = READ_CONSTANT();
                push(constant);
                // printValue(constant);
                // printf("\n");
                break;
            }
            case OP_RETURN: {
                printf("\nResult: ");
                printValue(pop());
                printf("\n");
                return INTERPRET_OK;
            }
            case OP_NEGATE: {
                push(-pop()); 
                break;
            }
            case OP_ADD:      BINARY_OP(+); break;
            case OP_SUBTRACT: BINARY_OP(-); break;
            case OP_MULTIPLY: BINARY_OP(*); break;
            case OP_DIVIDE:   BINARY_OP(/); break;
        }
    }

#undef READ_BYTE
#undef READ_CONSTANT
#undef BINARY_OP
}

InterpretResult interpret(const char* source) {
  compile(source);
  return INTERPRET_OK;
}


void push(Value value) {
  *vm.stackTop = value;
  vm.stackTop++;
}

Value pop() {
  vm.stackTop--;
  return *vm.stackTop;
}