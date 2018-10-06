arc4
====

A small and insanely fast ARCFOUR (RC4) cipher implementation of Python.

* Strongly focused on performance; entire source code is written in C.
* Easily installable; single file with no dependency.

Benchmark
---------

Below is benchmark metrics against 3 major RC4 implementations.

arc4 is 36 % faster than the de facto `PyCrypto <https://pypi.org/project/pycrypto/>`_ library.
Also, 1554 % faster than pure-Python `rc4 <https://pypi.org/project/rc4/>`_ library.

========= ==============
arc4      0.399230957031
PyCrypto  0.544879198074
rc4       6.60579204559
========= ==============

The whole benchmark code is in ``./benchmark.py``.

Install
-------

Clone the repo and do install::

   git clone https://github.com/manicmaniac/arc4.git
   cd arc4
   python setup.py install

Usage
-----

.. code:: python

   from arc4 import ARC4

   arc4 = ARC4('key')
   cipher = arc4.encrypt('some plain text to encrypt')

Because RC4 is a stream cipher, you must initialize RC4 object in the beginning of each operations.

.. code:: python

   arc4 = ARC4('key')
   arc4.decrypt(cipher)

Testing
-------

.. code:: python

   python -m unittest discover

License
-------

This software is under the MIT License.
