#ifndef clox_memory_h
#define clox_memory_h

#include "common.h"

#define GROW_CAPACITY(capacity) \
    ((capacity) < 8 ? 8 : (capacity) * 2)

#define GROW_ARRAY(type, pointer, oldCount, newCount) \
    (type*)reallocate(pointer, sizeof(type) * (oldCount), \
        sizeof(type) * (newCount))

#define FREE_ARRAY(type, pointer, oldCount) \
    reallocate(pointer, sizeof(type) * (oldCount), 0)

// written by ChatGPT
// #define INIT_OFFSETS(type, pointer, oldCount, newCount)                     \
//     ({                                                                      \
//         type* _newPointer = (type*)reallocate(pointer,                      \
//             sizeof(type) * (oldCount), sizeof(type) * (newCount));          \
//         if (_newPointer) {                                                  \
//             for (size_t _i = (oldCount); _i < (newCount); ++_i) {           \
//                 _newPointer[_i] = (type)(-1);                               \
//             }                                                               \
//         }                                                                   \
//         _newPointer;                                                        \
//     })

void* reallocate(void* pointer, size_t oldSize, size_t newSize);

#endif