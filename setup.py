try:
    from setuptools import Extension, setup
except ImportError:
    from distutils.core import Extension, setup

setup(
    name='arc4',
    version='0.0.1',
    description=(
        'A small and insanely fast ARCFOUR (RC4) cipher implementation ' +
        'of Python'
    ),
    author='Ryosuke Ito',
    author_email='rito.0305@gmail.com',
    ext_modules=[
        Extension('arc4', sources=['arc4.c'], extra_compile_args=['-O3'])
    ],
    classifiers=[
        'Development Status :: 4 - Beta',
        'Intended Audience :: Developers',
        'License :: OSI Approved :: MIT License',
        'Operating System :: Unix',
        'Operating System :: Microsoft :: Windows',
        'Operating System :: MacOS :: MacOS X',
        'Programming Language :: Python :: 2.7',
        'Programming Language :: Python :: 3.5',
        'Programming Language :: Python :: 3.6',
        'Programming Language :: Python :: Implementation :: CPython',
        'Topic :: Security :: Cryptography',
    ],
)
