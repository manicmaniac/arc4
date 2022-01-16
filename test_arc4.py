# coding:utf-8

from __future__ import unicode_literals

import doctest
import functools
import sys
import unittest

import arc4

KEY = b'PYTHON3'

LOREM = b"""Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do \
eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim \
veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea \
commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit \
esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat \
non proident, sunt in culpa qui officia deserunt mollit anim id est laborum."""

LOREM_ARC4 = b"""\xf0\xa8\x59\xec\xdf\x9d\xbd\x95\x52\x91\x66\x72\x50\x01\x0d\
\x3a\xac\x62\x10\xdc\x58\x0f\x49\x02\xd9\x45\x2a\xad\x3a\x2b\x79\xd5\x2b\x29\
\xe7\x16\xf1\x9c\x93\x58\xcd\xa9\x32\x87\xfc\x9f\x6e\x29\x14\x0a\x59\x12\x21\
\x89\x51\x49\xc7\x3f\x59\x78\x0b\x16\xb6\xb2\xc4\xc3\xc0\x61\xc4\xcd\xcf\x9e\
\xff\x34\x2c\xf2\x28\x14\xf8\xc9\x08\xf0\x1f\x2d\xfa\xe8\xbf\x77\xe0\xeb\xee\
\xa1\x51\xd4\xf3\x86\x66\x60\x1c\xb1\x3a\x14\x86\xf2\x6c\xe5\x47\xf8\xb5\x50\
\xad\xbc\x1c\x64\xeb\xbc\x52\x33\x60\x41\x58\x33\x6f\x58\x8c\xfd\x41\x1b\xb0\
\x05\xb3\xbc\x46\x37\xf3\xa4\x5e\x3e\x1f\x20\xe9\x00\x02\xcc\x31\x07\xe8\x65\
\xbb\x12\x97\x05\xcb\xfd\xba\x50\x9c\x59\x14\x49\xb4\x3c\x12\x2b\x47\x27\x5f\
\x30\x52\x57\xf4\xa2\x70\xc5\x7d\x4a\xf2\x92\x01\x5d\x02\x69\x1d\x74\xff\x43\
\xb1\x73\xb9\x28\xfe\x73\x62\x7f\xbd\xcd\xa1\x53\xa2\x1e\x28\x37\x19\xc4\x59\
\xbc\x81\x93\x79\x05\x13\x07\xc2\x43\xb3\xd1\x2a\x9d\xf7\x3c\xe7\x1e\x63\x4b\
\x70\xc7\xc2\xa6\x80\x31\xc7\xc5\x07\x64\x49\x40\x08\x7a\x4f\x4f\x90\x63\x88\
\x4d\x35\x8b\xd2\x48\xe1\xc2\xfc\xa2\xb5\x47\xca\xaf\x75\x36\x31\x22\xa8\x45\
\x5d\x0f\x03\xb7\xd5\x3b\xff\x47\xbc\x6f\xe0\xa3\x49\xfb\x63\xbe\xfc\xa7\x60\
\x59\x43\x50\x8e\x95\x76\x68\xda\xfa\xdb\x9b\x96\x9d\x1b\x6d\xac\x14\x2c\x12\
\x29\xfd\xf0\xaf\xc4\xba\x12\xdf\x83\xd9\xae\xcc\x19\x80\xfd\xc2\x36\x32\xf4\
\x01\x0b\x6d\xeb\x9e\xff\x74\x2e\xfe\x58\xc7\x91\xa9\x75\xf5\xa0\xc0\x5d\xb7\
\x5e\x6a\x71\x5a\x9c\xd3\x98\xca\x6c\xae\x80\xd6\x0d\xb9\x84\x63\x7f\xdf\x31\
\x1b\x5c\x4f\x07\x4c\x9b\x23\x24\x43\xce\x9e\x4d\x29\x5f\xb9\x3a\x57\x0f\x18\
\xf5\xa0\x5a\x94\x88\xfa\x55\x64\xca\x4f\x74\x9f\x71\x33\xa5\x6d\xd4\xd8\x5a\
\xdd\x51\x66\xad\xf5\x37\xad\x44\xe9\x20\xf2\x31\xd3\x9a\xef\x3e\x47\xd1\x20\
\x88\x2c\x21\x74\xed\xa3\x5c\x7c\xa7\x03\x42\x4d\x21\x50\xe2\x9b\x2b\x99\x88\
\x1e\xd4\x53\xda\x1c\xa2\xc7\x5b\xb5\x94\x5d\xc0"""

def raises_unicode_encode_error_on_python_2(f):
    if sys.version_info.major >= 3:
        return f
    @functools.wraps(f)
    def decorated(self, *args, **kwargs):
        with self.assertRaises(UnicodeEncodeError):
            return f(self, *args, **kwargs)
    return decorated


class TestARC4(unittest.TestCase):
    def test_init_with_zero_length_key_raises_error(self):
        with self.assertRaises(ValueError) as e:
            arc4.ARC4(b'')
            self.assertEqual('invalid key length: 0', e.message)

    def test_init_with_bytes_returns_instance(self):
        self.assertIsInstance(arc4.ARC4(b'spam'), arc4.ARC4)

    @raises_unicode_encode_error_on_python_2
    def test_init_with_unicode_returns_instance(self):
        self.assertIsInstance(arc4.ARC4(u'スパム'), arc4.ARC4)

    def test_init_with_bytearray_raises_type_error(self):
        with self.assertRaises(TypeError):
            arc4.ARC4(bytearray([0x66, 0x6f, 0x6f]))

    def test_init_with_memoryview_raises_type_error(self):
        with self.assertRaises(TypeError):
            arc4.ARC4(memoryview(b'spam'))

    def test_encrypt_with_long_bytes_returns_encrypted_bytes(self):
        cipher = arc4.ARC4(KEY)
        self.assertEqual(LOREM_ARC4, cipher.encrypt(LOREM))

    @raises_unicode_encode_error_on_python_2
    def test_encrypt_with_unicode_returns_encrypted_bytes(self):
        cipher = arc4.ARC4(b'spam')
        self.assertEqual(b'Q\xcd\xb1!\xecg', cipher.encrypt(u'ハム'))

    def test_encrypt_with_bytearray_raises_type_error(self):
        cipher = arc4.ARC4(b'spam')
        with self.assertRaises(TypeError):
            cipher.encrypt(bytearray(b'ham'))

    def test_encrypt_with_memoryview_raises_type_error(self):
        cipher = arc4.ARC4(b'spam')
        with self.assertRaises(TypeError):
            cipher.encrypt(memoryview(b'ham'))

    def test_encrypt_with_list_raises_type_error(self):
        cipher = arc4.ARC4(b'spam')
        with self.assertRaises(TypeError):
            cipher.encrypt([0x68, 0x61, 0x6d])

    def test_decrypt_with_long_bytes_returns_decrypted_bytes(self):
        cipher = arc4.ARC4(KEY)
        self.assertEqual(LOREM, cipher.decrypt(LOREM_ARC4))


def load_tests(loader, tests, ignore):
    tests.addTests(doctest.DocTestSuite(arc4))
    return tests
