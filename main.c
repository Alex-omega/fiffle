#include "common.h"
#include "chunk.h"
#include "debug.h"

int main(int argc, const char* argv[]) {
    Chunk chunk;
    initChunk(&chunk);
    int constant1 = addConstant(&chunk, 1.2);
    int constant2 = addConstant(&chunk, 1145);
    writeChunk(&chunk, OP_CONSTANT, 123);
    writeChunk(&chunk, constant1, 123);
    writeChunk(&chunk, OP_RETURN, 123);
    writeChunk(&chunk, OP_CONSTANT, 124);
    writeChunk(&chunk, constant2, 124);
    writeChunk(&chunk, OP_RETURN, 124);
    writeChunk(&chunk, OP_CONSTANT, 125);
    writeChunk(&chunk, constant2, 125);
    writeChunk(&chunk, OP_RETURN, 125);
    writeChunk(&chunk, OP_CONSTANT, 126);
    writeChunk(&chunk, constant2, 126);
    writeChunk(&chunk, OP_RETURN, 126);
    writeChunk(&chunk, OP_CONSTANT, 127);
    writeChunk(&chunk, constant2, 127);
    writeChunk(&chunk, OP_RETURN, 127);
        
    disAssembleChunk(&chunk, "test");
    freeChunk(&chunk);
    return 0;
}