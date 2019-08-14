#define PY_SSIZE_T_CLEAN
#include <Python.h>

typedef struct arc4_state {
    unsigned char x;
    unsigned char y;
    unsigned char s[256];
} arc4_state;

static void arc4_init(arc4_state *state, const unsigned char *key, Py_ssize_t keylen) {
    int i;
    unsigned char j, k;

    for (i = 0; i < 256; i++) {
        state->s[i] = (unsigned char)i;
    }
    state->x = 0;
    state->y = 0;
    j = 0;
    for (i = 0; i < 256; i++) {
        j += state->s[i] + key[i % keylen];
        k = state->s[i];
        state->s[i] = state->s[j];
        state->s[j] = k;
    }
}

static void arc4_crypt(arc4_state *state, unsigned char *buf, Py_ssize_t buflen) {
    unsigned char x;
    unsigned char y;
    unsigned char *s;
    Py_ssize_t i;
    unsigned char sx;
    unsigned char sy;

    x = state->x;
    y = state->y;
    s = state->s;
    for (i = 0; i < buflen; i++) {
        x++;
        y += s[x];
        sx = s[x];
        sy = s[y];
        s[x] = sy;
        s[y] = sx;
        buf[i] ^= s[(sx + sy) & 0xFF];
    }
    state->x = x;
    state->y = y;
}

typedef struct arc4_ARC4 {
    PyObject_HEAD
    arc4_state state;
} arc4_ARC4Object;

static int arc4_ARC4_init(arc4_ARC4Object *self, PyObject *args, PyObject *kwargs) {
    const char *key = NULL;
    Py_ssize_t keylen = 0;

    if (!PyArg_ParseTuple(args, "s#", &key, &keylen)) {
        return -1;
    }
    arc4_init(&(self->state), (const unsigned char *)key, keylen);
    return 0;
}

static PyObject *arc4_ARC4_crypt(arc4_ARC4Object *self, PyObject *args) {
    const char *bufstring = NULL;
    char *buf = NULL;
    Py_ssize_t buflen = 0;
    PyObject *rv = NULL;

    if (!PyArg_ParseTuple(args, "s#:crypt", &bufstring, &buflen)) {
        return NULL;
    }
    buf = PyMem_Malloc(sizeof(char) * buflen);
    memcpy(buf, bufstring, buflen);
    arc4_crypt(&(self->state), (unsigned char *)buf, buflen);
    rv = PyBytes_FromStringAndSize((const char *)buf, buflen);
    PyMem_Free(buf);
    return rv;
}

static PyMethodDef arc4_ARC4_methods[] = {
    { "decrypt", (PyCFunction)arc4_ARC4_crypt, METH_VARARGS, "" },
    { "encrypt", (PyCFunction)arc4_ARC4_crypt, METH_VARARGS, "" },
    { NULL }
};

static PyTypeObject arc4_ARC4Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "arc4.ARC4",
    sizeof(arc4_ARC4Object),
    0, /* tp_itemsize */
    0, /* tp_dealloc */
    0, /* tp_print */
    0, /* tp_getattr */
    0, /* tp_setattr */
    0, /* tp_compare */
    0, /* tp_repr */
    0, /* tp_as_number */
    0, /* tp_as_sequence */
    0, /* tp_as_mapping */
    0, /* tp_hash */
    0, /* tp_call */
    0, /* tp_str */
    0, /* tp_getattro */
    0, /* tp_setattro */
    0, /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT, /* tp_flags */
    "", /* tp_doc */
    0, /* tp_traverse */
    0, /* tp_clear */
    0, /* tp_richcompare */
    0, /* tp_weaklistoffset */
    0, /* tp_iter */
    0, /* tp_iternext */
    arc4_ARC4_methods, /* tp_methods */
    0, /* tp_members */
    0, /* tp_getset */
    0, /* tp_base */
    0, /* tp_dict */
    0, /* tp_descr_get */
    0, /* tp_descr_set */
    0, /* tp_dictoffset */
    (initproc)arc4_ARC4_init, /* tp_init */
    0, /* tp_alloc */
    PyType_GenericNew, /* tp_new */
};

#if PY_MAJOR_VERSION >= 3
static struct PyModuleDef arc4_module = {
    PyModuleDef_HEAD_INIT,
    "arc4",
    NULL,
    -1,
    NULL
};

PyMODINIT_FUNC PyInit_arc4(void) {
    PyObject *m;

    if (PyType_Ready(&arc4_ARC4Type) < 0) {
        return NULL;
    }
    m = PyModule_Create(&arc4_module);
    if (m == NULL) {
        return NULL;
    }
    Py_INCREF(&arc4_ARC4Type);
    PyModule_AddObject(m, "ARC4", (PyObject *)&arc4_ARC4Type);
    return m;
}
#else
PyMODINIT_FUNC initarc4(void) {
    PyObject *m;

    if (PyType_Ready(&arc4_ARC4Type) < 0) {
        return;
    }
    m = Py_InitModule("arc4", NULL);
    if (m == NULL) {
        return;
    }
    Py_INCREF(&arc4_ARC4Type);
    PyModule_AddObject(m, "ARC4", (PyObject *)&arc4_ARC4Type);
}
#endif /* PY_MAJOR_VERSION >= 3 */
