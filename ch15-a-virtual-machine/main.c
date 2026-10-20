#include <stdio.h>
#include <time.h>

#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"

int main(int argc, const char* argv[]) {
    initVM();

    Chunk chunk;
    initChunk(&chunk);

    writeConstant(&chunk, 1.2, 123);
    writeConstant(&chunk, 3.4, 123);
    writeChunk(&chunk, OP_ADD, 123);

    writeConstant(&chunk, 5.6, 123);
    writeChunk(&chunk, OP_DIVIDE, 123);
    writeChunk(&chunk, OP_NEGATE, 123);

    writeChunk(&chunk, OP_RETURN, 123);

    disassembleChunk(&chunk, "test chunk");
    interpret(&chunk);

    Chunk big;
    initChunk(&big);

    for (int i = 0; i < 300; i++) {
        writeConstant(&big, 1, 124);
    }
    for (int i = 0; i < 299; i++) {
        writeChunk(&big, OP_ADD, 124);
    }
    writeChunk(&big, OP_RETURN, 124);

    interpret(&big);
    printf("stack grew to %d slots\n", vm.stackCapacity);
    freeChunk(&big);

    Chunk bench;
    initChunk(&bench);

    writeConstant(&bench, 1, 125);
    for (int i = 0; i < 50000000; i++) {
        writeChunk(&bench, OP_NEGATE, 125);
    }
    writeChunk(&bench, OP_RETURN, 125);

    clock_t start = clock();
    interpret(&bench);
    double seconds = (double)(clock() - start) / CLOCKS_PER_SEC;
    printf("50 million negates took %.3f seconds\n", seconds);
    freeChunk(&bench);

    freeVM();
    freeChunk(&chunk);
    return 0;
}
