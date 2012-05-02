#include <Python.h>
#include <iostream>
using namespace std;


class Test {
    public:
        int a;
        
};

static PyObject * print_hello_world(PyObject *self, PyObject *args){

    // create a set containing "hello" & "world"
    
    PyObject *set = PySet_New(NULL);

    PySet_Add(set, Py_BuildValue("s","hello"));
    PySet_Add(set, Py_BuildValue("s","world"));

    return Py_BuildValue("O", set);

}

static PyObject * dict_in_r_dict_out(PyObject *self, PyObject *args){
    
    // expect an dict, will return it with the values swapped with the keys
    
    PyObject *dict_out = PyDict_New();

    PyObject *dict_in;
    PyObject *callback = NULL;
    
    // iterator over items in dict_in
    PyObject *key, *value;
    Py_ssize_t pos = 0;
    if (PyArg_UnpackTuple(args, "ref", 1, 2, &dict_in, &callback)) {
        
        // insert, and swap key & value
        while (PyDict_Next(dict_in, &pos, &key, &value ) ) {
            PyDict_SetItem(dict_out, value, key);
        }
    }

    return Py_BuildValue("O", dict_out);
    
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
