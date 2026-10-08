package com.craftinginterpreters.lox;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

class LoxClass extends LoxInstance implements LoxCallable {
    final String name;
    final LoxClass superclass;
    private final Map<String, LoxFunction> methods;

    LoxClass(LoxClass metaclass, String name, LoxClass superclass,
             Map<String, LoxFunction> methods) {
        super(metaclass);
        this.name = name;
        this.superclass = superclass;
        this.methods = methods;
    }

    LoxFunction findMethod(LoxInstance instance, String name) {
        List<LoxFunction> chain = new ArrayList<>();
        for (LoxClass klass = this; klass != null; klass = klass.superclass) {
            if (klass.methods.containsKey(name)) {
                chain.add(klass.methods.get(name));
            }
        }

        if (chain.isEmpty()) return null;

        LoxCallable inner = new NoInner(chain.get(0).arity());
        LoxFunction bound = null;
        for (LoxFunction method : chain) {
            bound = method.bind(instance, inner);
            inner = bound;
        }
        return bound;
    }

    private static class NoInner implements LoxCallable {
        private final int arity;

        NoInner(int arity) {
            this.arity = arity;
        }

        @Override
        public int arity() {
            return arity;
        }

        @Override
        public Object call(Interpreter interpreter, List<Object> arguments) {
            return null;
        }

        @Override
        public String toString() {
            return "<no inner>";
        }
    }

    @Override
    public Object call(Interpreter interpreter, List<Object> arguments) {
        LoxInstance instance = new LoxInstance(this);
        LoxFunction initializer = findMethod(instance, "init");
        if (initializer != null) {
            initializer.call(interpreter, arguments);
        }
        return instance;
    }

    @Override
    public int arity() {
        LoxFunction initializer = findMethod(null, "init");
        if (initializer == null) return 0;
        return initializer.arity();
    }

    @Override
    public String toString() {
        return name;
    }
}
