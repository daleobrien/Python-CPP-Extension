#include <Python.h>
#include "structmember.h"

#include <iostream>
using namespace std;

class SomeClass {

  public:
    int i;
    int j;
    int k;
    SomeClass():i(0),j(0),k(0){};
    void unit(){
        this->i = 1;
        this->j = 1;
        this->k = 1;
    }
    ~SomeClass(){};
};

typedef struct {
    PyObject_HEAD
    SomeClass * klass; // some external class, which may have all kinds of stuff in it, e.g. non-POD
                       // class vars that are exposed are placed into 4 places in the code below ... numbered (1) - (4)
} SimpleObject;

static PyObject * Unit(PyObject *obj, PyObject *args,  PyObject *kwds ){
    ( (SimpleObject*) obj)->klass->unit();
    return Py_BuildValue("");
}

static PyMethodDef simpleObjectMethods[] = {
    {"unit", (PyCFunction)Unit, METH_NOARGS, "Unit" },
    {NULL}
};

static PyObject *
simpleObjectNew(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    SimpleObject *self;

    self = (SimpleObject *)type->tp_alloc(type, 0);
    if (self != NULL) {
        SomeClass *x = new SomeClass;
        self->klass = x;
    }

    return (PyObject *)self;
}

int convert_to_an_int_from_Int_or_Str(PyObject* in, void* out){
    // return 0 if an error occured, else 1

    if( in==NULL){
        // no errors, no parameter
        return 1;
    }

    // get them and convert, where posible, should check limits, e.t.c, convert strings, and so on...
    if( in && PyInt_Check(in) ){

       *(int *)out = PyInt_AS_LONG(in);
        return 1;

    } else if( PyString_Check(in) ){

        PyObject *x = PyInt_FromString( PyBytes_AsString(in),0,10);

        long z = PyInt_AsLong(x);
        if ( PyErr_Occurred()  == NULL ){
            *( int *)out = z;
            return 1;
        } else{
            PyErr_SetString(PyExc_TypeError, "Cannot set 'i', expecting an INT type, got a string type ");
            return 0;
        }
    }
    // can't convert, ...
    PyErr_SetString(PyExc_TypeError, "Cannot set 'i', expecting an INT type, got a ... type.");
    return 0;


}

static int
simpleObjectInit(SimpleObject *self, PyObject *args, PyObject *kwds){

    // (1) ***  update klass variables directly
    static char *kwlist[] = {(char*)"i", (char*)"j",(char*)"k", NULL};

    if (! PyArg_ParseTupleAndKeywords(args, kwds, "|O&O&O&", kwlist,
           convert_to_an_int_from_Int_or_Str, &(self->klass->i),
           convert_to_an_int_from_Int_or_Str, &(self->klass->j),
           convert_to_an_int_from_Int_or_Str, &(self->klass->k)) ){
        return -1;
    }

    return 0;
}


static int
simpleObjectTraverse(SimpleObject *self, visitproc visit, void *arg) {
    //Py_VISIT(self->xxx);
    return 0;
}

static int
simpleObjectClear(SimpleObject *self) {

    if(self->klass){
        delete self->klass;
        self->klass=NULL;
    }
    return 0;
}

static void
simpleObjectDealloc(SimpleObject* self){

    simpleObjectClear(self);
    self->ob_type->tp_free((PyObject*)self);
}


static PyMemberDef simpleObjectMembers[] = {
    // (2) *** expose klass variables to python
    {(char *)"i", T_INT, -1, 0, (char *)"i factor"}, // just so it shows up with the python dir command
    {(char *)"j", T_INT, -1, 0, (char *)"j factor"}, // they are never called for setting/getting directly
    {(char *)"k", T_INT, -1, 0, (char *)"k factor"}, // Note: can't used offset() for a non-POD class

    {(char *)"t", T_INT, -1, 0, (char *)"the tuple"}, // Note: can't used offset() for a non-POD class
    {(char *)"l", T_INT, -1, 0, (char *)"the list"}, // Note: can't used offset() for a non-POD class
   // {(char *)"variable", T_INT, offsetof(SimpleObject, x), 0, (char *)"variable"},
    {NULL}
};

int simpleObjectSetAttrO(PyObject *obj, PyObject *name, PyObject *value){

    SimpleObject *self = (SimpleObject *) obj;

    char* variable = PyBytes_AsString(name);

    // (3) *** over-ride setters,  redirect some to klass variables
    if(strncmp( variable, "i",1) ==0){ return ( convert_to_an_int_from_Int_or_Str(value, &self->klass->i))==0?-1:0; }
    if(strncmp( variable, "j",1) ==0){ return ( convert_to_an_int_from_Int_or_Str(value, &self->klass->j))==0?-1:0; }
    if(strncmp( variable, "k",1) ==0){ return ( convert_to_an_int_from_Int_or_Str(value, &self->klass->k))==0?-1:0; }

    // default setter for everything else
    return  PyObject_GenericSetAttr(obj,name,value);
}

static PyObject*
simpleObjectGetAttrO(PyObject *obj, PyObject* name){

    SimpleObject *self = (SimpleObject *) obj;
    char* variable = PyBytes_AsString(name);

    // (4) *** over-ride setters,  redirect some to klass variables
    if(strncmp( variable, "i",1) ==0){ PyObject *r = PyInt_FromLong(self->klass->i ); return r; }
    if(strncmp( variable, "j",1) ==0){ PyObject *r = PyInt_FromLong(self->klass->j ); return r; }
    if(strncmp( variable, "k",1) ==0){ PyObject *r = PyInt_FromLong(self->klass->k ); return r; }

    // Some containers
    if(strncmp( variable, "t",1) ==0){ return Py_BuildValue("(iii)", 3, 2, 1); }
    if(strncmp( variable, "l",1) ==0){ return Py_BuildValue("[iii]", 3, 2, 1); }

    // default getter for everything else
    return PyObject_GenericGetAttr(obj, name);
}


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
    simpleObjectGetAttrO,      /*tp_getattro*/
    simpleObjectSetAttrO,      /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_HAVE_GC,
    "Simple object docs ...",
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

    Py_INCREF(&simpleObjectType);
    PyModule_AddObject(m, "Simple", (PyObject *)&simpleObjectType);

}

int main(void) {
    return 0;
}
