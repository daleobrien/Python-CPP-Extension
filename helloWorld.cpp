#include <Python.h>
#include "structmember.h"

#include <iostream>
using namespace std;

typedef struct {
    PyObject_HEAD
    long i; // Could point to my external class, ... then I can keep it simple :)
    long j;
    long k;
} SimpleObject;

static PyMethodDef simpleObjectMethods[] = {
   // {"name", (PyCFunction)Noddy_name, METH_NOARGS, "Return the name, combining the first and last name" },
    {NULL}  
};

static PyObject *
simpleObjectNew(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    SimpleObject *self;

    self = (SimpleObject *)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->i = 0;
        self->j = 0;
        self->k = 0;
    }

    return (PyObject *)self;
}


static int
simpleObjectInit(SimpleObject *self, PyObject *args, PyObject *kwds){

    PyObject *i=NULL, *j=NULL, *k=NULL;

    static char *kwlist[] = {(char*)"i", (char*)"j",(char*)"k", NULL};

    if (! PyArg_ParseTupleAndKeywords(args, kwds, "|OOO", kwlist, &i, &j, &k)){
        return -1; 
    }

    // get them and convert, where posible, should check limits, e.t.c, convert strings, and so on...
    if( i && PyInt_Check(i) ){ self->i = PyInt_AS_LONG(i); }
    else if( i && PyFloat_Check(i) ){ self->i = PyFloat_AsDouble(i); }
    else if( i && PyString_Check(i) ){

        //PyObject* pStrObj = PyUnicode_AsUTF8String(i);
        char* zStr = PyBytes_AsString(i);
        cout << zStr << endl;
        //char* zStrDup = strdup(zStr);
        //Py_DECREF(pStrObj);
        //cout << zStrDup << endl;

        //PyObject *x = PyInt_FromString(zStrDup,NULL,10);
        //self->i = PyInt_AS_LONG(x); 
        //Py_DECREF(x);
    }
    else if( i ){ 
        // can't convert, ... 
        PyErr_SetString(PyExc_TypeError, "Cannot set 'i', expecting an INT type, got a ... type.");
        return -1;
        // 
    }


    if( j && PyInt_Check(j) ){ self->j = PyInt_AS_LONG(j); }
    if( k && PyInt_Check(k) ){ self->k = PyInt_AS_LONG(k); }

    return 0;
}


static int
simpleObjectTraverse(SimpleObject *self, visitproc visit, void *arg) {
    //Py_VISIT(self->xxx);
    return 0;
}

static int 
simpleObjectClear(SimpleObject *self) {
    return 0;
}

static void
simpleObjectDealloc(SimpleObject* self){
    simpleObjectClear(self);
    self->ob_type->tp_free((PyObject*)self);
}



static PyMemberDef simpleObjectMembers[] = {
    {(char *)"i", T_INT, offsetof(SimpleObject, i), 0, (char *)"i factor"},
    {(char *)"j", T_INT, offsetof(SimpleObject, j), 0, (char *)"j factor"},
    {(char *)"k", T_INT, offsetof(SimpleObject, k), 0, (char *)"k factor"},
    {NULL} 
};

static PyTypeObject simpleObjectType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "Simple Object",           /*tp_name*/
    sizeof(SimpleObject),      /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)simpleObjectDealloc, /*tp_dealloc*/
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
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_HAVE_GC, 
    "Simple object docs",    
    (traverseproc)simpleObjectTraverse,	
    (inquiry)simpleObjectClear,
    0,		                   /* tp_richcompare */
    0,		                   /* tp_weaklistoffset */
    0,		                   /* tp_iter */
    0,		                   /* tp_iternext */
    simpleObjectMethods,      
    simpleObjectMembers,     
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)simpleObjectInit,
    0,                         /* tp_alloc */
    simpleObjectNew,           
    
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
    {"printHelloWorld",  print_hello_world, METH_VARARGS, "printHelloWorld Documentation"},
    {"dict_to_dict",  dict_in_r_dict_out, METH_VARARGS, "Swaps values & keys, (will kill dups values)"},
    {NULL, NULL, 0, NULL}
};


PyMODINIT_FUNC inithelloWorld(void){

    if (PyType_Ready(&simpleObjectType) < 0){ return; }

    PyObject* m = Py_InitModule3("helloWorld", HelloWorldMethods,"Module Documentation");

    //simpleObjectType.tp_new = PyType_GenericNew;

    Py_INCREF(&simpleObjectType);
    PyModule_AddObject(m, "Simple", (PyObject *)&simpleObjectType);

}

int main(void) {
    return 0;
}
