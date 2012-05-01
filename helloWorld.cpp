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

static PyObject * dict_in_r_dict_out(PyObject *self, PyObject *args){
    
    // TODO: extract dict from input 

    PyObject *dict = PyDict_New();

    PyObject *x = PyFloat_FromDouble(1.0); 
    PyObject *z = PyFloat_FromDouble(2.0); 

    PyDict_SetItem(dict, x, z);

    Py_DECREF(x);
    Py_DECREF(z);

    return Py_BuildValue("O", dict);
    
}



static PyMethodDef HelloWorldMethods[] = {
    {"printHelloWorld",  print_hello_world, METH_VARARGS, "Prints Hello World"},
    {"dict_to_dict",  dict_in_r_dict_out, METH_VARARGS, "Swaps values & keys, (will kill dups values)"},
    {NULL, NULL, 0, NULL}
};



PyMODINIT_FUNC inithelloWorld(void){
    (void) Py_InitModule("helloWorld", HelloWorldMethods);
}

int main(void) {
    return 0;
}
