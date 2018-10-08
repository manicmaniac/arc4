from __future__ import unicode_literals

import unittest

from arc4 import ARC4


class TestARC4(unittest.TestCase):
    def test_encrypt(self):
        arc4 = ARC4(b'foo')
        self.assertEqual(b'\xc9C\x9f', arc4.encrypt(b'bar'))

    def test_decrypt(self):
        arc4 = ARC4(b'foo')
        self.assertEqual(b'bar', arc4.encrypt(b'\xc9C\x9f'))
