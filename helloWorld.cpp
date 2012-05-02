#include <Python.h>
#include <iostream>
using namespace std;


typedef class {
    PyObject_HEAD
    /* Type-specific fields go here. */
    int k;
} SimpleObject;

static PyTypeObject simpleObjectType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "Simple Object",           /*tp_name*/
    sizeof(SimpleObject),      /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    0,                         /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT,        /*tp_flags*/
    "Simple object",           /* tp_doc */
    0,		               /* tp_traverse */
    0,		               /* tp_clear */
    0,		               /* tp_richcompare */
    0,		               /* tp_weaklistoffset */
    0,		               /* tp_iter */
    0,		               /* tp_iternext */
    0,             /* tp_methods */
    0,             /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    0,      /* tp_init */
    0,                         /* tp_alloc */
    NULL,                 /* tp_new */
    
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

    PyObject* m = Py_InitModule("helloWorld", HelloWorldMethods);

    simpleObjectType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&simpleObjectType) < 0){ return; }

    Py_INCREF(&simpleObjectType);
    PyModule_AddObject(m, "Simple", (PyObject *)&simpleObjectType);

}

int main(void) {
    return 0;
}
