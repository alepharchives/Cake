/*  
    Copyright (C) 2011 Cake authors,
    
    This file is part of Cake.
    
    Cake is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Cake is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "closure.h"

#include <stdlib.h>

struct closure {
    void *(*function)(void *env, void *arg);
    void (*free)(void *env);
    void *(*copy)(void *env);
    void *data;
};

static void *run_function(void *env, void *arg) {
    void *(*function)(void *) = env;
    return function(arg);
}

static void no_op_free(void *arg) {}

static void *shallow_copy(void *arg) {
    return arg;
}

closure *create_closure(void *(*fn)(void *, void *),
                        void (*fr)(),
                        void *(*cp)(void *),
                        void *dat) {
    closure *res = malloc(sizeof(closure));
    res->function = fn;
    res->free = fr;
    res->copy = cp;
    res->data = dat;
    return res;
}

closure *function_to_closure(void *(*function)(void *)) {
    return create_closure(run_function, no_op_free, shallow_copy, function);
}

void *call_closure(closure *cl, void *arg) {
    return cl->function(cl->data, arg);
}

void free_closure(closure *cl) {
    cl->free(cl->data);
    free(cl);
}

closure *copy_closure(closure *cl) {
    return create_closure(cl->function, cl->free, cl->copy, cl->copy(cl->data));
}
