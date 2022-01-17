arc4
====

.. image:: https://github.com/manicmaniac/arc4/actions/workflows/test.yml/badge.svg
   :target: https://github.com/manicmaniac/arc4/actions/workflows/test.yml
   :alt: Test

.. image:: https://readthedocs.org/projects/arc4/badge/?version=latest
   :target: https://arc4.readthedocs.io/en/latest/?badge=latest
   :alt: Documentation Status

.. image:: https://api.codeclimate.com/v1/badges/e7c21db66865a6d487d0/maintainability
   :target: https://codeclimate.com/github/manicmaniac/arc4/maintainability
   :alt: Maintainability

A small and insanely fast ARCFOUR (RC4) cipher implementation of Python.

* Strongly focused on performance; entire source code is written in C.
* Easily installable; single file with no dependency.

Benchmark
---------

Below is benchmark metrics against 3 major RC4 implementations.

.. image:: https://user-images.githubusercontent.com/1672393/149827568-ca0c699a-ab26-4821-a8c6-6f3578a0c679.png
   :alt: Encrypt 1,024KB * 400 times

.. table:: Encrypt 1,024KB * 400 times

================================================= =========== ===========
                     Library                        1 thread   4 threads
================================================= =========== ===========
`arc4 <https://pypi.org/project/arc4/>`_            0.803 sec   0.589 sec
`PyCrypto <https://pypi.org/project/pycrypto/>`_    1.861 sec   0.858 sec
`rc4 <https://pypi.org/project/rc4/>`_            210.643 sec 213.000 sec
================================================= =========== ===========

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
