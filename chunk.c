#include <stdlib.h>

#include "chunk.h"
#include "memory.h"

void initChunk(Chunk* chunk) {
    chunk->count = 0; // 计数, 表明当前占用到chunk中第几段内存
    chunk->capacity = 0; // 容量, 表明当前chunk中最多能容纳多少段内存
    chunk->code = NULL;
    chunk->line_capacity = 0;
    chunk->line_count = 0;
    chunk->line_offesets = NULL;
    chunk->line0 = -1; // 暂时用-1表示无效行号
    // chunk->lines = NULL;
    initValueArray(&chunk->constants);
}

void writeChunk(Chunk* chunk, uint8_t byte, int line) { // 核心函数
    // 若容量不足, 先扩容 (不做实际操作)
    if (chunk->capacity < chunk->count + 1) { // 小于比等于更快
        int oldCapacity = chunk->capacity; // 保存旧容量
        chunk->capacity = GROW_CAPACITY(oldCapacity); // 计算需要的新容量
        chunk->code = GROW_ARRAY(uint8_t, chunk->code,
            oldCapacity, chunk->capacity); // 扩容
        // chunk->lines = GROW_ARRAY(int, chunk->lines,
        //     oldCapacity, chunk->capacity);
    }
    if (chunk->line_capacity < chunk->line_count + 1) {
        int oldCapacity = chunk->line_capacity;
        chunk->line_capacity = GROW_CAPACITY(oldCapacity);
        chunk->line_offesets = GROW_ARRAY(int, chunk->line_offesets,
            oldCapacity, chunk->line_capacity);
    }

    // 在count(当前计数)处写入byte数据, 然后更新平行的lines array
    chunk->code[chunk->count] = byte; // 正常写入数据
    // chunk->lines[chunk->count] = line;
    if (chunk->line0 == -1) {
        chunk->line0 = line;
    }
    chunk->line_offesets[line - chunk->line0]++;
    if (chunk->line_offesets[line - chunk->line0] == 1) {
        chunk->line_count++;
    }

    chunk->count++;
}

void freeChunk(Chunk* chunk) {
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    FREE_ARRAY(int, chunk->line_offesets, chunk->line_capacity);
    freeValueArray(&chunk->constants);
    initChunk(chunk);
}

int addConstant(Chunk* chunk, Value value) {
    /* Add value to the ConBlock of the chunk, and return the index of the new value (in the constant block) */
  writeValueArray(&chunk->constants, value);
  return chunk->constants.count - 1;
}