#include <stdio.h>

#include "common.h"
#include "chunk.h"
#include "debug.h"

int main(int argc, const char* argv[]) {
    Chunk chunk;
    initChunk(&chunk);

    int constant = addConstant(&chunk, 1.2);
    writeChunk(&chunk, OP_CONSTANT, 123);
    writeChunk(&chunk, constant, 123);

    writeChunk(&chunk, OP_RETURN, 123);

    constant = addConstant(&chunk, 4.5);
    writeChunk(&chunk, OP_CONSTANT, 124);
    writeChunk(&chunk, constant, 124);
    writeChunk(&chunk, OP_RETURN, 125);
    writeChunk(&chunk, OP_RETURN, 125);
    writeChunk(&chunk, OP_RETURN, 125);

    disassembleChunk(&chunk, "test chunk");
    printf("%d bytes stored with %d line entries\n", chunk.count, chunk.lineCount);
    freeChunk(&chunk);
    return 0;
}
