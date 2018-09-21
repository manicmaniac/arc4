from __future__ import unicode_literals

from string import ascii_letters
from timeit import timeit
from random import choice

BYTES = ''.join(choice(ascii_letters) for i in range(1024))
NUMBER = 50000

if __name__ == '__main__':
    arc4 = timeit('arc4.ARC4("key").encrypt("{}")'.format(BYTES), 'import arc4', number=NUMBER)
    pycrypto = timeit('ARC4.new("key").encrypt("{}")'.format(BYTES), 'import Crypto.Cipher.ARC4 as ARC4', number=NUMBER)
    rc4 = timeit('rc4.rc4("key", "bytes")', 'import rc4', number=NUMBER)
    print('arc4: {}'.format(arc4))
    print('pycrypto: {}'.format(pycrypto))
    print('rc4: {}'.format(rc4))
