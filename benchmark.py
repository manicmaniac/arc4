from __future__ import unicode_literals

from string import ascii_letters
from timeit import timeit
from random import choice

from matplotlib.ticker import FormatStrFormatter, AutoMinorLocator
import matplotlib.pyplot as plt

BYTES = ''.join(choice(ascii_letters) for _ in range(1024))
NUMBER = 50000


def export_graph(path, labels, values, style):
    with plt.style.context(style):
        fig, ax = plt.subplots()
        ax.set_title('Seconds for decrypting 1KB * {:,} times (lower is better)'.format(NUMBER))
        ax.set_ylabel('Seconds')
        if style == 'bmh':
            ax.xaxis.grid()
        else:
            ax.yaxis.grid()
        ax.yaxis.set_major_formatter(FormatStrFormatter('%.1f'))
        ax.yaxis.set_minor_locator(AutoMinorLocator())
        ax.bar(result.keys(), result.values())
        for i, v in enumerate(result.values()):
            ax.text(i - 0.1, v + 0.05, '{:.3} sec'.format(v))
        fig.savefig(path)


if __name__ == '__main__':
    result = {
        'arc4': timeit('arc4.ARC4("key").encrypt("{}")'.format(BYTES),
                       'import arc4', number=NUMBER),
        'pycrypto': timeit('ARC4.new("key").encrypt("{}")'.format(BYTES),
                           'import Crypto.Cipher.ARC4 as ARC4', number=NUMBER),
        'rc4': timeit('rc4.rc4("key", "bytes")', 'import rc4', number=NUMBER)
    }
    for k, v in result.items():
        print('{}: {}'.format(k, v))
    export_graph('benchmark.png', result.keys(), result.values(), 'bmh')
