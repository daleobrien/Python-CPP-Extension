#include <Python.h>
#include <iostream>
using namespace std;

static PyObject * print_hello_world(PyObject *self, PyObject *args){

    // create a set containing "hello" & "world"
    PyObject *set = PySet_New(NULL);
    PySet_Add(set, Py_BuildValue("s","hello"));
    PySet_Add(set, Py_BuildValue("s","world"));
    return Py_BuildValue("O", set);

}

static PyMethodDef HelloWorldMethods[] = {
    {"printHelloWorld",  print_hello_world, METH_VARARGS, "Prints hello World"},
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC inithelloWorld(void){
    (void) Py_InitModule("helloWorld", HelloWorldMethods);
}

int main(void) {
    return 0;
}
