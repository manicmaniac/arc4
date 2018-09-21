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
)
