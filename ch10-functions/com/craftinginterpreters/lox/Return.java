package com.craftinginterpreters.lox;

class Return extends RuntimeException {
    final Object value;

    Return(Object value) {
        // No message or cause, and skip stack traces since this is control flow, not an error.
        super(null, null, false, false);
        this.value = value;
    }
}
