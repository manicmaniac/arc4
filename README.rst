arc4
====

.. image:: https://github.com/manicmaniac/arc4/actions/workflows/test.yml/badge.svg
   :target: https://github.com/manicmaniac/arc4/actions/workflows/test.yml
   :alt: Test

.. image:: https://readthedocs.org/projects/arc4/badge/?version=latest
   :target: https://arc4.readthedocs.io/en/latest/?badge=latest
   :alt: Documentation Status

A small and insanely fast ARCFOUR (RC4) cipher implementation of Python.

* Strongly focused on performance; entire source code is written in C.
* Easily installable; single file with no dependency.

Benchmark
---------

Below is benchmark metrics against 3 major RC4 implementations.

.. image:: https://user-images.githubusercontent.com/1672393/149629214-79b44d7b-fa0a-4d28-bb93-4ab8604aef44.png
   :alt: Seconds for decrypting 1KB * 50,000 times

.. table:: Seconds for decrypting 1KB * 50,000 times

================================================= =========
`arc4 <https://pypi.org/project/arc4/>`_          0.212 sec
`PyCrypto <https://pypi.org/project/pycrypto/>`_  0.511 sec
`rc4 <https://pypi.org/project/rc4/>`_            3.72  sec
================================================= =========

The whole benchmark code is in ``./benchmark.py``.

Install
-------

Install from PyPI::

   pip install arc4

Or clone the repo and do install::

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

Documents
---------

Here is the API reference.

https://arc4.readthedocs.io/en/latest/

Testing
-------

.. code:: python

   python -m unittest discover

License
-------

This software is under the MIT License.
