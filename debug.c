#include <stdio.h>

#include "debug.h"
#include "value.h"

int simpleInstruction(const char* name, int offset) {
    printf("%s\n", name);
    return offset + 1;
}

int constantInstruction(const char* name, Chunk* chunk, int offset) {
    uint8_t constant = chunk->code[offset + 1];
    printf("%-16s", name);
    printValue(chunk->constants.values[constant]);
    printf("\tfrom values[%d] in chunk\n", constant);
    return offset + 2;
}

void disAssembleChunk(Chunk* chunk, const char* name) {
    printf("== %s ==\n", name);

    for (int offset = 0; offset < chunk->count;) {
        offset = disAssembleInstruction(chunk, offset);
    }
}

int disAssembleInstruction(Chunk* chunk, int offset) {
    printf("%04d ", offset);
    if (offset > 0 &&
        !isLineAhead(chunk, offset)) {
        printf(" --- ");
    } else {
        printf("%4d ", getLine(chunk, offset));
    }
    uint8_t instruction = chunk->code[offset];
    switch (instruction) {
        case OP_CONSTANT:
            return constantInstruction("OP_CONSTANT", chunk, offset);
        case OP_RETURN:
            return simpleInstruction("OP_RETURN", offset);
        case OP_NEGATE:
            return simpleInstruction("OP_NEGATE", offset);
        case OP_ADD:
            return simpleInstruction("OP_ADD", offset);
        case OP_SUBTRACT:
            return simpleInstruction("OP_SUBTRACT", offset);
        case OP_MULTIPLY:
            return simpleInstruction("OP_MULTIPLY", offset);
        case OP_DIVIDE:
            return simpleInstruction("OP_DIVIDE", offset);
        default:
            printf("Unknown opcode %d\n", instruction);
            return offset + 1;
    }
}


int getLine(Chunk* chunk, int offset) {
    if (offset < 0) {
        return chunk->line0 + offset;
    }
    int* line_offsets = chunk->line_offesets;
    int total = 0;
    for (int i = 0; i < chunk->line_count; i++) {
        total += line_offsets[i];
        if (total > offset) {
            return chunk->line0 + i;
        }
    }
}

bool isLineAhead(Chunk* chunk, int offset) {
    return getLine(chunk, offset) > getLine(chunk, offset - 1);
}