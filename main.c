#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include "basic.h"
#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"

// 内联函数，获取时间戳计数器值
static __inline__ uint64_t rdtsc() {
    uint32_t lo, hi;
    __asm__ __volatile__("rdtsc" : "=a"(lo), "=d"(hi));
    return ((uint64_t)hi << 32) | lo;
}

int main(int argc, const char* argv[]) {
    uint64_t start, end; // 定义用于存储 TSC 时间戳的变量

    // 获取起始时间戳
    start = rdtsc();

    initVM();
/*
    // Chunk chunk;
    // initChunk(&chunk);
    // int constant = addConstant(&chunk, 3);
    // writeChunk(&chunk, OP_CONSTANT, 123);
    // writeChunk(&chunk, constant, 123);

    // constant = addConstant(&chunk, 2);
    // writeChunk(&chunk, OP_CONSTANT, 123);
    // writeChunk(&chunk, constant, 123);

    // writeChunk(&chunk, OP_SUBTRACT, 123);

    // writeChunk(&chunk, OP_NEGATE, 123);

    // writeChunk(&chunk, OP_RETURN, 123);

    // interpret(&chunk);
*/

    if (argc == 1) {
        repl();
    } else if (argc == 2) {
        runFile(argv[1]);
    } else {
        fprintf(stderr, "Usage: clox [path]\n");
        exit(64);
    }


    freeVM();
    // freeChunk(&chunk);

    // 获取结束时间戳
    end = rdtsc();

    // 计算并输出总运行时间
    printf("程序运行时间（TSC 周期数）: %lu\n", end - start);

    // 如果需要转换为秒，可以使用 CPU 主频计算（假设主频为 3.0GHz）
    double cpu_hz = 3.0e9; // 修改为你的实际 CPU 主频
    printf("程序运行时间（秒）: %.10f\n", (end - start) / cpu_hz);

    return 0;
}


static void repl() {
    char line[1024];
    for (;;) {
        printf("> ");
        if (!fgets(line, sizeof(line), stdin)) {
            printf("\n");
            break;
        }
        interpret(line);
    }
}

static void runFile(const char* path) {
    char* source = readFile(path);
    InterpretResult result = interpret(source);
    free(source); 

    if (result == INTERPRET_COMPILE_ERROR) exit(65);
    if (result == INTERPRET_RUNTIME_ERROR) exit(70);
}

static char* readFile(const char* path) {
    FILE* file = fopen(path, "rb");

    if (file == NULL) {
        fprintf(stderr, "Could not open file \"%s\".\n", path);
        exit(74);
    }

    fseek(file, 0L, SEEK_END);
    size_t fileSize = ftell(file);
    rewind(file);

    char* buffer = (char*)malloc(fileSize + 1);
    if (buffer == NULL) {
        fprintf(stderr, "Not enough memory to read \"%s\".\n", path);
        exit(74);
    }
    size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
    if (bytesRead < fileSize) {
        fprintf(stderr, "Could not read file \"%s\".\n", path);
        exit(74);
    }
    buffer[bytesRead] = '\0';

    fclose(file);
    return buffer;
}