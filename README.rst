arc4
====

.. image:: https://github.com/manicmaniac/arc4/actions/workflows/test.yml/badge.svg
   :target: https://github.com/manicmaniac/arc4/actions/workflows/test.yml
   :alt: Test

.. image:: https://qlty.sh/gh/manicmaniac/projects/arc4/coverage.svg
   :target: https://qlty.sh/gh/manicmaniac/projects/arc4
   :alt: Test Coverage

.. image:: https://qlty.sh/gh/manicmaniac/projects/arc4/maintainability.svg
   :target: https://qlty.sh/gh/manicmaniac/projects/arc4
   :alt: Maintainability

.. image:: https://readthedocs.org/projects/arc4/badge/?version=latest
   :target: https://arc4.readthedocs.io/en/latest/?badge=latest
   :alt: Documentation Status

A small and insanely fast ARCFOUR (RC4) cipher implementation of Python.

* Strongly focused on performance; entire source code is written in C.
* Thread-safety; you can improve further performance with multi-threading.
* Easily installable; single file with no dependency, pre-built wheels provided.

Benchmark
---------

Below is benchmark metrics against 3 major RC4 implementations.

.. image:: https://gist.githubusercontent.com/manicmaniac/991d0c197e1cb62eec81edec65363187/raw/benchmark.svg
   :alt: Benchmark

The whole benchmark code is in ``./benchmark.py``.

.. note::

   arc4 supports multi-threading but it is too fast to ignore the overhead of context-switching, in most cases.

Install
-------

Install from PyPI

.. code:: sh

   pip install arc4

Or clone the repo and do install

.. code:: sh

   git clone https://github.com/manicmaniac/arc4.git
   cd arc4
   python setup.py install

Usage
-----

.. code:: python

   >>> from arc4 import ARC4
   >>> arc4 = ARC4(b'key')
   >>> cipher = arc4.encrypt(b'some plain text to encrypt')

Because RC4 is a stream cipher, you must initialize RC4 object in the beginning of each operations.

.. code:: python

   >>> arc4 = ARC4(b'key')
   >>> arc4.decrypt(cipher)
   b'some plain text to encrypt'

Documents
---------

Here is the API reference.

https://arc4.readthedocs.io/en/latest/

Testing
-------

.. code:: sh

   python -m unittest discover

License
-------

This software is under the MIT License.
