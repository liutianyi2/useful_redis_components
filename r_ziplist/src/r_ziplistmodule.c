#include <Python.h>
#include "zmalloc.h"
#include "ziplist.h"
#include <assert.h>

struct list_manager{
    int list_count;
    int list_capacity;
    unsigned char **lists;
};

static struct list_manager l_m = {0, 0, NULL};

static int list_add(unsigned char *l){
    if(l_m.list_capacity == 0){
        l_m.lists = zmalloc(20 * sizeof(unsigned char *));
        assert(l_m.lists != NULL);
        l_m.list_capacity = 20;
    }
    if(l_m.list_capacity == (l_m.list_count + 1)){
        l_m.lists = zrealloc(l_m.lists, (l_m.list_capacity * 2) * sizeof(unsigned char *));
        assert(l_m.lists != NULL);
        l_m.list_capacity *= 2;
    }
    *(l_m.lists + l_m.list_count) = l;
    l_m.list_count++;

    return l_m.list_count - 1;
}

static unsigned char *list_get(int index){
    if(index < 0 || index >= l_m.list_count || l_m.lists[index] == NULL)
        return NULL;
    return l_m.lists[index];
}

static void *list_set(int index, unsigned char *new_l){
    l_m.lists[index] = new_l;
}

static void list_free(int index){
    if(index >= l_m.list_count)
        return;
    zfree(l_m.lists[index]);
    l_m.lists[index] = NULL;
}

static PyObject* r_ziplist_new(PyObject *self, PyObject *args){
    unsigned char *zl = ziplistNew();
    assert(zl != NULL);
    int index = list_add(zl);

    return Py_BuildValue("i", index);
}

static PyObject* r_ziplist_append(PyObject* self, PyObject *args){
    char *s;
    unsigned char *l, *new_l;
    int index;
    if(!PyArg_ParseTuple(args, "is", &index, &s)){
        return NULL;
    }
    l = list_get(index);
    if(l == NULL)
        return NULL;
    new_l = ziplistPush(l, (unsigned char *)s, strlen(s), ZIPLIST_TAIL);
    if(new_l != l)
        list_set(index, new_l);
    return Py_BuildValue("i", 1);
}

static PyObject* r_ziplist_cursor(PyObject* self, PyObject *args){
    unsigned char *l, *p;
    int index;
    if(!PyArg_ParseTuple(args, "i", &index)){
        return NULL;
    }
    l = list_get(index);
    if(l == NULL)
        return NULL;
    p = ziplistIndex(l, 0);
    if(p == NULL)
        return NULL;

    return Py_BuildValue("L", (char *)p);
}

static PyObject* r_ziplist_traverse(PyObject* self, PyObject *args){
    unsigned char *p;
    int index, res;
    unsigned int slen;
    long long l;
    unsigned char *s;
    static char out_str[1000];

    if(!PyArg_ParseTuple(args, "iL", &index, (long long **)&p)){
        return NULL;
    }
    res = ziplistGet(p, &s, &slen, &l);
    if(res == 0)
        return Py_None;
    p = ziplistNext(list_get(index), p);
    if(s != NULL){
        strncpy(out_str, (char *)s, slen);
        return Py_BuildValue("Ls", (char *)p, out_str);
    }else{
        return Py_BuildValue("Li", (char *)p, l);
    }
}

static char new_docs[] = "new\n";
static char append_docs[] = "append\n";
static char cursor_docs[] = "cursor\n";
static char traverse_docs[] = "traverse\n";

static PyMethodDef r_ziplist_methods[] = {
    {"new", (PyCFunction)r_ziplist_new, METH_VARARGS, new_docs},
    {"append", (PyCFunction)r_ziplist_append, METH_VARARGS, append_docs},
    {"cursor", (PyCFunction)r_ziplist_cursor, METH_VARARGS, cursor_docs},
    {"traverse", (PyCFunction)r_ziplist_traverse, METH_VARARGS, traverse_docs},
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC initr_ziplist(void){
    Py_InitModule3("r_ziplist", r_ziplist_methods, "redis source code based ziplist demo");
}
