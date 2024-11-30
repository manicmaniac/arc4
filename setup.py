try:
    from setuptools import Extension, setup
except ImportError:
    from distutils.core import Extension, setup


VERSION = '0.5.0'


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
        package_data={
            'arc4-stubs': ['__init__.pyi'],
        },
        packages=['arc4-stubs'],
        keywords=[
            'ARCFOUR',
            'cipher',
            'cryptography',
            'RC4',
        ],
        classifiers=[
            'Development Status :: 4 - Beta',
            'License :: OSI Approved :: MIT License',
            'Operating System :: OS Independent',
            'Programming Language :: Python :: 3.9',
            'Programming Language :: Python :: 3.10',
            'Programming Language :: Python :: 3.11',
            'Programming Language :: Python :: 3.12',
            'Programming Language :: Python :: 3.13',
            'Programming Language :: Python :: Implementation :: CPython',
            'Programming Language :: Python :: Implementation :: PyPy',
            'Topic :: Security :: Cryptography',
        ],
    )
