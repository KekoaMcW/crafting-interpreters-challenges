package com.craftinginterpreters.lox;

import java.util.ArrayList;
import java.util.List;

class LoxList extends LoxInstance {
    private final List<Object> elements = new ArrayList<>();

    LoxList() {
        super(null);
    }

    @Override
    Object get(Token name) {
        switch (name.lexeme) {
            case "length":
                return (double)elements.size();

            case "add":
                return new Native(1) {
                    @Override
                    public Object call(Interpreter interpreter, List<Object> arguments) {
                        elements.add(arguments.get(0));
                        return null;
                    }
                };

            case "get":
                return new Native(1) {
                    @Override
                    public Object call(Interpreter interpreter, List<Object> arguments) {
                        return elements.get(index(name, arguments.get(0)));
                    }
                };

            case "set":
                return new Native(2) {
                    @Override
                    public Object call(Interpreter interpreter, List<Object> arguments) {
                        elements.set(index(name, arguments.get(0)), arguments.get(1));
                        return null;
                    }
                };

            case "remove":
                return new Native(1) {
                    @Override
                    public Object call(Interpreter interpreter, List<Object> arguments) {
                        return elements.remove(index(name, arguments.get(0)));
                    }
                };
        }

        throw new RuntimeError(name,
                "Undefined property '" + name.lexeme + "' on list.");
    }

    @Override
    void set(Token name, Object value) {
        throw new RuntimeError(name, "Can't add properties to a list.");
    }

    private int index(Token name, Object value) {
        if (!(value instanceof Double)) {
            throw new RuntimeError(name, "List index must be a number.");
        }

        double number = (double)value;
        int index = (int)number;
        if (index != number) {
            throw new RuntimeError(name, "List index must be a whole number.");
        }

        if (index < 0 || index >= elements.size()) {
            throw new RuntimeError(name, "List index " + index +
                    " out of bounds for length " + elements.size() + ".");
        }

        return index;
    }

    @Override
    public String toString() {
        StringBuilder builder = new StringBuilder("[");
        for (int i = 0; i < elements.size(); i++) {
            if (i > 0) builder.append(", ");
            builder.append(Interpreter.stringify(elements.get(i)));
        }
        builder.append("]");
        return builder.toString();
    }

    private abstract static class Native implements LoxCallable {
        private final int arity;

        Native(int arity) {
            this.arity = arity;
        }

        @Override
        public int arity() {
            return arity;
        }

        @Override
        public String toString() {
            return "<native fn>";
        }
    }
}
