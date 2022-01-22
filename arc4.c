#define PY_SSIZE_T_CLEAN
#include <Python.h>

#ifndef __restrict
#if __STDC_VERSION__ >= 199901L
#define __restrict restrict
#else
#define __restrict
#endif /* __STDC_VERSION >= 199901L */
#endif /* __restrict */

struct arc4_state {
    unsigned char x, y, s[256];
};

static void
arc4_init(struct arc4_state *state, const unsigned char *key,
          Py_ssize_t key_size)
{
    int i;
    unsigned char j, k;

    state->x = 0;
    state->y = 0;
    for (i = 0; i < 256; i++) {
        state->s[i] = (unsigned char)i;
    }
    j = 0;
    for (i = 0; i < 256; i++) {
        j += state->s[i] + key[i % key_size];
        k = state->s[i];
        state->s[i] = state->s[j];
        state->s[j] = k;
    }
}

static void
arc4_crypt(struct arc4_state *state, const unsigned char *__restrict input,
           unsigned char *__restrict output, Py_ssize_t size)
{
    unsigned char x, y, *s, sx, sy;
    Py_ssize_t i;

    x = state->x;
    y = state->y;
    s = state->s;
    for (i = 0; i < size; i++) {
        x++;
        y += s[x];
        sx = s[x];
        sy = s[y];
        s[x] = sy;
        s[y] = sx;
        output[i] = input[i] ^ s[(sx + sy) & 0xFF];
    }
    state->x = x;
    state->y = y;
}

struct arc4_ARC4 {
    PyObject_HEAD
    struct arc4_state state;
};

static int
arc4_ARC4_init(struct arc4_ARC4 *self, PyObject *args, PyObject *kwargs)
{
    const char *key = NULL;
    Py_ssize_t key_size = 0;

    if (!PyArg_ParseTuple(args, "s#", &key, &key_size)) {
        return -1;
    }
    if (key_size <= 0) {
        PyErr_Format(PyExc_ValueError, "invalid key length: %zd", key_size);
        return -1;
    }
    Py_BEGIN_ALLOW_THREADS
        arc4_init(&(self->state), (const unsigned char *)key, key_size);
    Py_END_ALLOW_THREADS
    return 0;
}

static int
byteslike_as_string_and_size(PyObject *obj, char **buffer, Py_ssize_t *size)
{
#if PY_MAJOR_VERSION >= 3
    const char *utf8;
#else
    PyObject *ascii;
#endif

    if (PyBytes_Check(obj)) {
        *buffer = PyBytes_AS_STRING(obj);
        *size = PyBytes_GET_SIZE(obj);
        return 0;
    }
    if (PyUnicode_Check(obj)) {
#if PY_MAJOR_VERSION >= 3
        utf8 = (char *)PyUnicode_AsUTF8AndSize(obj, size);
        if (utf8 != NULL) {
            *buffer = (char *)utf8;
            return 0;
        }
#else
        ascii = PyUnicode_AsASCIIString(obj);
        if (ascii != NULL) {
            *buffer = PyBytes_AS_STRING(ascii);
            *size = PyBytes_GET_SIZE(ascii);
            Py_DECREF(ascii);
            return 0;
        }
#endif
    }
    return -1;
}

static PyObject *
arc4_ARC4_crypt(struct arc4_ARC4 *self, PyObject *arg)
{
    const char *input = NULL;
    char *output = NULL;
    Py_ssize_t size = 0;
    PyObject *outputBytes = NULL;

    if (arg == NULL) {
        return NULL;
    }
    if (byteslike_as_string_and_size(arg, (char **)&input, &size) == -1) {
        if (PyErr_Occurred() == NULL) {
            PyErr_Format(PyExc_TypeError,
                         "crypt() argument 1 must be read-only bytes-like "
                         "object, not %s",
                         arg->ob_type->tp_name);
        }
        return NULL;
    }
    outputBytes = PyBytes_FromStringAndSize(NULL, size);
    output = PyBytes_AS_STRING(outputBytes);
    Py_BEGIN_ALLOW_THREADS
        arc4_crypt(&(self->state), (const unsigned char *)input,
                   (unsigned char *)output, size);
    Py_END_ALLOW_THREADS
    return outputBytes;
}

PyDoc_STRVAR(arc4_ARC4_decrypt_doc,
             "decrypt(data: bytes) -> bytes\n"
             "\n"
             "Decrypt cipher with a given key.\n"
             "\n"
             "Parameters\n"
             "----------\n"
             "data : bytes\n"
             "    Encrypted data.\n"
             "\n"
             "Returns\n"
             "----------\n"
             "bytes\n"
             "    Decrypted data.\n"
             "\n"
             "Notes\n"
             "----------\n"
             "This method is defined for readability although it is identical "
             "to :func:`~arc4.ARC4.encrypt`.\n"
             "\n"
             "Examples\n"
             "--------\n"
             ">>> arc4 = ARC4(b'spam')\n"
             ">>> arc4.decrypt(b'\\xda/S')\n"
             "b'ham'\n");

PyDoc_STRVAR(arc4_ARC4_encrypt_doc,
             "encrypt(data: bytes) -> bytes\n"
             "\n"
             "Encrypt cipher with a given key.\n"
             "\n"
             "Parameters\n"
             "----------\n"
             "data : bytes\n"
             "    Decrypted data.\n"
             "\n"
             "Returns\n"
             "----------\n"
             "bytes\n"
             "    Encrypted data.\n"
             "\n"
             "Notes\n"
             "----------\n"
             "This method is defined for readability although it is identical "
             "to :func:`~arc4.ARC4.decrypt`.\n"
             "\n"
             "Examples\n"
             "--------\n"
             ">>> arc4 = ARC4(b'spam')\n"
             ">>> arc4.encrypt(b'ham')\n"
             "b'\\xda/S'\n");

static PyMethodDef arc4_ARC4_methods[] = {
    {"decrypt", (PyCFunction)arc4_ARC4_crypt, METH_O, arc4_ARC4_decrypt_doc},
    {"encrypt", (PyCFunction)arc4_ARC4_crypt, METH_O, arc4_ARC4_encrypt_doc},
    {NULL}};

PyDoc_STRVAR(arc4_ARC4Type_doc,
             "A class represents a session of RC4 stream cipher.\n"
             "\n"
             "__init__(key: bytes)\n"
             "\n"
             "Parameters\n"
             "----------\n"
             "key : bytes\n"
             "    A key to encrypt or decrypt.\n"
             "\n"
             "Raises\n"
             "------\n"
             "ValueError\n"
             "    When the key length is zero.\n"
             "\n"
             "Notes\n"
             "-----\n"
             "You have to initialize an instance in the beginning of each "
             "operations.\n");

static PyTypeObject arc4_ARC4Type = {
    PyVarObject_HEAD_INIT(NULL, 0) /* A trailing comma is included */
    "arc4.ARC4",
    sizeof(struct arc4_ARC4),
    0,                        /* tp_itemsize */
    0,                        /* tp_dealloc */
    0,                        /* tp_print */
    0,                        /* tp_getattr */
    0,                        /* tp_setattr */
    0,                        /* tp_compare */
    0,                        /* tp_repr */
    0,                        /* tp_as_number */
    0,                        /* tp_as_sequence */
    0,                        /* tp_as_mapping */
    0,                        /* tp_hash */
    0,                        /* tp_call */
    0,                        /* tp_str */
    0,                        /* tp_getattro */
    0,                        /* tp_setattro */
    0,                        /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,       /* tp_flags */
    arc4_ARC4Type_doc,        /* tp_doc */
    0,                        /* tp_traverse */
    0,                        /* tp_clear */
    0,                        /* tp_richcompare */
    0,                        /* tp_weaklistoffset */
    0,                        /* tp_iter */
    0,                        /* tp_iternext */
    arc4_ARC4_methods,        /* tp_methods */
    0,                        /* tp_members */
    0,                        /* tp_getset */
    0,                        /* tp_base */
    0,                        /* tp_dict */
    0,                        /* tp_descr_get */
    0,                        /* tp_descr_set */
    0,                        /* tp_dictoffset */
    (initproc)arc4_ARC4_init, /* tp_init */
    0,                        /* tp_alloc */
    PyType_GenericNew,        /* tp_new */
};

#if PY_MAJOR_VERSION >= 3
static struct PyModuleDef arc4_module = {PyModuleDef_HEAD_INIT, "arc4", NULL,
                                         -1, NULL};

PyMODINIT_FUNC
PyInit_arc4(void)
{
    PyObject *module;

    if (PyType_Ready(&arc4_ARC4Type) < 0) {
        return NULL;
    }
    module = PyModule_Create(&arc4_module);
    if (module == NULL) {
        return NULL;
    }
    Py_INCREF(&arc4_ARC4Type);
    PyModule_AddObject(module, "ARC4", (PyObject *)&arc4_ARC4Type);
    return module;
}
#else
PyMODINIT_FUNC
initarc4(void)
{
    PyObject *module;

    if (PyType_Ready(&arc4_ARC4Type) < 0) {
        return;
    }
    module = Py_InitModule("arc4", NULL);
    if (module == NULL) {
        return;
    }
    Py_INCREF(&arc4_ARC4Type);
    PyModule_AddObject(module, "ARC4", (PyObject *)&arc4_ARC4Type);
}
#endif /* PY_MAJOR_VERSION >= 3 */
