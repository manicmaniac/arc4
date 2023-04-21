#define PY_SSIZE_T_CLEAN
#include <Python.h>

/* "restrict" qualifier and "inline" function specifier were introduced in C99.
 * See http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1124.pdf
 *
 * Major compilers have alternatives of these keywords available even in C89.
 */
#if __STDC_VERSION__ < 199901L
#ifdef __restrict
#define restrict __restrict
#else
#define restrict
#endif /* __restrict */
#ifdef __inline__
#define inline __inline__
#else
#define inline
#endif /* __inline__ */
#endif /* __STDC_VERSION__ < 199901L */

struct arc4_state {
    unsigned char x, y, s[256];
};

static const struct arc4_state arc4_initial_state = {
    0, /* x */
    0, /* y */
    {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b,
     0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
     0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23,
     0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
     0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b,
     0x3c, 0x3d, 0x3e, 0x3f, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
     0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51, 0x52, 0x53,
     0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f,
     0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b,
     0x6c, 0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77,
     0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f, 0x80, 0x81, 0x82, 0x83,
     0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
     0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0x9b,
     0x9c, 0x9d, 0x9e, 0x9f, 0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
     0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3,
     0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf,
     0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb,
     0xcc, 0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7,
     0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf, 0xe0, 0xe1, 0xe2, 0xe3,
     0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef,
     0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb,
     0xfc, 0xfd, 0xfe, 0xff} /* s */
};

static inline void
arc4_init(struct arc4_state *state, const unsigned char *key,
          Py_ssize_t key_size)
{
    register unsigned char *s = state->s;
    register int i;
    register unsigned char j = 0;

    memcpy(state, &arc4_initial_state, sizeof(*state));
    for (i = 0; i < 256; i++) {
        register unsigned char k = s[i];

        j += k + key[i % key_size];
        s[i] = s[j];
        s[j] = k;
    }
}

static inline void
arc4_crypt(struct arc4_state *state, const unsigned char *restrict input,
           unsigned char *restrict output, Py_ssize_t size)
{
    register unsigned char x = state->x;
    register unsigned char y = state->y;
    {
        register unsigned char *s = state->s;
        register unsigned char sx, sy;
        register Py_ssize_t i;

        for (i = 0; i < size; i++) {
            x++;
            sx = s[x];
            y += sx;
            sy = s[y];
            s[x] = sy;
            s[y] = sx;
            output[i] = input[i] ^ s[(sx + sy) & 0xFF];
        }
    }
    state->x = x;
    state->y = y;
}

struct arc4_ARC4 {
    PyObject_HEAD
    struct arc4_state state;
};

static const char arc4_str_deprecation_warning[] =
    "passing an object other than 'bytes' as argument 1 is deprecated.";

static int
arc4_ARC4_init(struct arc4_ARC4 *self, PyObject *args, PyObject *kwargs)
{
    const char *key = NULL;
    Py_ssize_t key_size = 0;
    int is_bytes = 0;

    if (!PyArg_ParseTuple(args, "s#", &key, &key_size)) {
        return -1;
    }
    args = PyTuple_GET_ITEM(args, 0);
    is_bytes = PyBytes_Check(args);
    if (!is_bytes && PyErr_WarnEx(PyExc_DeprecationWarning,
                                  arc4_str_deprecation_warning, 1) == -1) {
        return -1;
    }
#ifdef PYPY_VERSION
    /* PyPy's PyArg_ParseTuple(_, "s#", _, _) treats bytearray and memoryview
     * as byte-like object.
     * This behavior looks intended as document says:
     * > a few corner cases don’t raise the same exception
     * https://doc.pypy.org/en/latest/cpython_differences.html#miscellaneous
     */
    if (!(is_bytes || PyUnicode_Check(args))) {
        PyErr_Format(PyExc_TypeError,
                     "argument 1 must be read-only bytes-like object, not %s",
                     Py_TYPE(args)->tp_name);
        return -1;
    }
#endif /* PYPY_VERSION */
    if (key_size <= 0) {
        PyErr_Format(PyExc_ValueError, "invalid key length: %zd", key_size);
        return -1;
    }
    Py_BEGIN_ALLOW_THREADS
        arc4_init(&(self->state), (const unsigned char *)key, key_size);
    Py_END_ALLOW_THREADS
    return 0;
}

static void
arc4_ARC4_dealloc(struct arc4_ARC4 *self)
{
    memset(&self->state, 0, sizeof(self->state));
    Py_TYPE(self)->tp_free((PyObject *)self);
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
    if (PyBytes_Check(arg)) {
        input = PyBytes_AS_STRING(arg);
        size = PyBytes_GET_SIZE(arg);
    }
    else if (PyUnicode_Check(arg)) {
        if (PyErr_WarnEx(PyExc_DeprecationWarning,
                         arc4_str_deprecation_warning, 1) == -1) {
            return NULL;
        }
        input = (char *)PyUnicode_AsUTF8AndSize(arg, &size);
        if (input == NULL) {
            return NULL;
        }
    }
    else {
        return PyErr_Format(PyExc_TypeError,
                            "crypt() argument 1 must be read-only bytes-like "
                            "object, not %s",
                            Py_TYPE(arg)->tp_name);
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
    {NULL} /* sentinel */
};

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

PyDoc_STRVAR(arc4_doc, "ARCFOUR (RC4) implementation in Python/C API.");

static PyTypeObject arc4_ARC4Type = {
    PyVarObject_HEAD_INIT(NULL, 0) /* A trailing comma is included */
    "arc4.ARC4",
    sizeof(struct arc4_ARC4),
    0,                             /* tp_itemsize */
    (destructor)arc4_ARC4_dealloc, /* tp_dealloc */
    0,                             /* tp_print */
    0,                             /* tp_getattr */
    0,                             /* tp_setattr */
    0,                             /* tp_compare */
    0,                             /* tp_repr */
    0,                             /* tp_as_number */
    0,                             /* tp_as_sequence */
    0,                             /* tp_as_mapping */
    0,                             /* tp_hash */
    0,                             /* tp_call */
    0,                             /* tp_str */
    0,                             /* tp_getattro */
    0,                             /* tp_setattro */
    0,                             /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,            /* tp_flags */
    arc4_ARC4Type_doc,             /* tp_doc */
    0,                             /* tp_traverse */
    0,                             /* tp_clear */
    0,                             /* tp_richcompare */
    0,                             /* tp_weaklistoffset */
    0,                             /* tp_iter */
    0,                             /* tp_iternext */
    arc4_ARC4_methods,             /* tp_methods */
    0,                             /* tp_members */
    0,                             /* tp_getset */
    0,                             /* tp_base */
    0,                             /* tp_dict */
    0,                             /* tp_descr_get */
    0,                             /* tp_descr_set */
    0,                             /* tp_dictoffset */
    (initproc)arc4_ARC4_init,      /* tp_init */
    0,                             /* tp_alloc */
    PyType_GenericNew,             /* tp_new */
};

static struct PyModuleDef arc4_module = {
    PyModuleDef_HEAD_INIT, /* m_base */
    "arc4",                /* m_name */
    arc4_doc,              /* m_doc */
    -1,                    /* m_size */
    NULL                   /* m_methods */
};

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
    if (PyModule_AddStringConstant(module, "__version__",
                                   Py_STRINGIFY(ARC4_VERSION)) < 0) {
        Py_DECREF(module);
        return NULL;
    }
    Py_INCREF(&arc4_ARC4Type);
    if (PyModule_AddObject(module, "ARC4", (PyObject *)&arc4_ARC4Type) < 0) {
        Py_DECREF(&arc4_ARC4Type);
        Py_DECREF(module);
        return NULL;
    }
    return module;
}
