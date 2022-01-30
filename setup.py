try:
    from setuptools import Extension, setup
except ImportError:
    from distutils.core import Extension, setup


VERSION = '0.2.0'


def read_file(path):
    with open(path) as f:
        return f.read()


if __name__ == '__main__':
    setup(
        name='arc4',
        version=VERSION,
        description=(
            'A small and insanely fast ARCFOUR (RC4) cipher implementation ' +
            'of Python'
        ),
        long_description=read_file('README.rst'),
        author='Ryosuke Ito',
        author_email='rito.0305@gmail.com',
        url='https://github.com/manicmaniac/arc4',
        ext_modules=[
            Extension('arc4',
                      sources=['arc4.c'],
                      define_macros=[('ARC4_VERSION', VERSION)]),
        ],
        classifiers=[
            'Development Status :: 4 - Beta',
            'License :: OSI Approved :: MIT License',
            'Operating System :: OS Independent',
            'Programming Language :: Python :: 2.7',
            'Programming Language :: Python :: 3.5',
            'Programming Language :: Python :: 3.6',
            'Programming Language :: Python :: 3.7',
            'Programming Language :: Python :: 3.8',
            'Programming Language :: Python :: 3.9',
            'Programming Language :: Python :: 3.10',
            'Programming Language :: Python :: Implementation :: CPython',
            'Programming Language :: Python :: Implementation :: PyPy',
            'Topic :: Security :: Cryptography',
        ],
    )
