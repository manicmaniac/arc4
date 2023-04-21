import argparse
import gc
import multiprocessing
import sys
import threading
import timeit

import Crypto.Cipher.ARC4
import arc4


TEXT = 'x' * 1024 * 1024
NCPU = multiprocessing.cpu_count()
N = NCPU * 100


def save_graph_image(path, labels, single_thread_values, multi_thread_values):
    import matplotlib
    if sys.version_info.major <= 2:
        matplotlib.use('TKAgg')
    import matplotlib.pyplot as plt
    width = 0.35
    xs = range(len(labels))
    with plt.style.context('bmh'):
        fig, ax = plt.subplots()
        single_thread_rects = ax.bar([x - width / 2 for x in xs],
                                     single_thread_values,
                                     width,
                                     label='1 thread')
        multi_thread_rects = ax.bar([x + width / 2 for x in xs],
                                    multi_thread_values,
                                    width,
                                    label='{} threads'.format(NCPU))
        ax.set_yscale('log')
        ax.set_ylabel('Seconds')
        kilobytes = len(TEXT) // 1024
        ax.set_title('Encrypt {:,}KB * {:,} times'.format(kilobytes, N))
        ax.set_xticks(xs)
        ax.set_xticklabels(labels)
        ax.legend()
        autolabel(ax, single_thread_rects, single_thread_values)
        autolabel(ax, multi_thread_rects, multi_thread_values)
        fig.tight_layout()
        fig.savefig(path, dpi=300)


def autolabel(ax, rects, values):
    """Attach a text label above each bar in *rects*, displaying its height."""
    for rect, value in zip(rects, values):
        ax.annotate('{:.3f}'.format(value),
                    xy=(rect.get_x() + rect.get_width() / 2, value),
                    xytext=(0, 3),  # 3 points vertical offset
                    textcoords='offset points',
                    ha='center',
                    va='bottom')


def benchmark(target, iterations, thread_count, timer=timeit.default_timer):
    assert 1 <= thread_count <= iterations
    result = 0
    if thread_count == 1:
        return _benchmark_single_thread(target, iterations, timer)
    else:
        return _benchmark_multi_thread(target, iterations, thread_count, timer)
    return result


def _benchmark_single_thread(target, iterations, timer):
    result = 0
    for _ in range(iterations):
        gc.disable()
        start = timer()
        target()
        result += timer() - start
        gc.enable()
    return result


def _benchmark_multi_thread(target, iterations, thread_count, timer):
    result = 0
    for _ in range(iterations // thread_count):
        threads = [threading.Thread(target=target)
                   for _ in range(thread_count)]
        gc.disable()
        start = timer()
        for thread in threads:
            thread.start()
        for thread in threads:
            thread.join()
        result += timer() - start
        gc.enable()
    return result


def arc4_code():
    arc4.ARC4('key').encrypt(TEXT)


def pycrypto_code():
    Crypto.Cipher.ARC4.new('key').encrypt(TEXT)


def rc4_code():
    rc4.rc4(TEXT, 'key')


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--skip-rc4',
                        action='store_true',
                        help='skip rc4 benchmark')
    parser.add_argument('-o',
                        '--output',
                        default=None,
                        help='path to write a graph')
    args = parser.parse_args()
    labels = ['arc4', 'pycrypto']
    single_thread_values = [
        benchmark(arc4_code, N, 1),
        benchmark(pycrypto_code, N, 1),
    ]
    multi_thread_values = [
        benchmark(arc4_code, N, NCPU),
        benchmark(pycrypto_code, N, NCPU),
    ]
    if not args.skip_rc4:
        import rc4
        labels.append('rc4')
        # rc4 is very slow so decrease N and multiply N to the result.
        single_thread_values.append(benchmark(rc4_code, NCPU, 1) * 100)
        multi_thread_values.append(
                benchmark(rc4_code, NCPU * 2, NCPU) * N // NCPU // 2)
    rates = [1, single_thread_values[1] / single_thread_values[0]]
    if not args.skip_rc4:
        rates.append(single_thread_values[2] / single_thread_values[0])
    result = zip(labels, single_thread_values, multi_thread_values, rates)
    print('library,1 thread,{} threads,rate'.format(NCPU))
    for label, single, multi, rate in result:
        print('{},{:g},{:g},{:g}'.format(label, single, multi, rate))
    if args.output:
        save_graph_image(args.output, labels, single_thread_values,
                         multi_thread_values)
