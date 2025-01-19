#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"
#include <stdint.h>
#include <stdio.h>

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
    Chunk chunk;
    initChunk(&chunk);
    int constant = addConstant(&chunk, 3);
    writeChunk(&chunk, OP_CONSTANT, 123);
    writeChunk(&chunk, constant, 123);

    constant = addConstant(&chunk, 2);
    writeChunk(&chunk, OP_CONSTANT, 123);
    writeChunk(&chunk, constant, 123);

    writeChunk(&chunk, OP_SUBTRACT, 123);

    writeChunk(&chunk, OP_NEGATE, 123);

    writeChunk(&chunk, OP_RETURN, 123);

    interpret(&chunk);

    freeVM();
    freeChunk(&chunk);

    // 获取结束时间戳
    end = rdtsc();

    // 计算并输出总运行时间
    printf("程序运行时间（TSC 周期数）: %lu\n", end - start);

    // 如果需要转换为秒，可以使用 CPU 主频计算（假设主频为 3.0GHz）
    double cpu_hz = 3.0e9; // 修改为你的实际 CPU 主频
    printf("程序运行时间（秒）: %.10f\n", (end - start) / cpu_hz);

    return 0;
}
