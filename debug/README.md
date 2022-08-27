## debug

### backtrace

Output example:

<pre>
$ ./backtrace
Started

Obtained 4 stack frames.
./backtrace(print_backtrace+0x2c) [0x5651a4290235]
./backtrace(main+0x24) [0x5651a429038b]
/lib/x86_64-linux-gnu/libc.so.6(__libc_start_main+0xf3) [0x7f321141d0b3]
./backtrace(_start+0x2e) [0x5651a429014e]

foo: depth = 0

Obtained 10 stack frames.
./backtrace(print_backtrace+0x2c) [0x5651a4290235]
./backtrace(foo+0x47) [0x5651a4290312]
./backtrace(bar+0x23) [0x5651a429033c]
./backtrace(foo+0x23) [0x5651a42902ee]
./backtrace(bar+0x23) [0x5651a429033c]
./backtrace(foo+0x23) [0x5651a42902ee]
./backtrace(bar+0x23) [0x5651a429033c]
./backtrace(foo+0x23) [0x5651a42902ee]
./backtrace(main+0x2e) [0x5651a4290395]
/lib/x86_64-linux-gnu/libc.so.6(__libc_start_main+0xf3) [0x7f321141d0b3]

</pre>

### callgrind

Output example:

<pre>
$ valgrind --tool=callgrind ./array_sort
==1896915== Callgrind, a call-graph generating cache profiler
==1896915== Copyright (C) 2002-2017, and GNU GPL'd, by Josef Weidendorfer et al.
==1896915== Using Valgrind-3.15.0 and LibVEX; rerun with -h for copyright info
==1896915== Command: ./array_sort
==1896915== 
==1896915== For interactive control, run 'callgrind_control -h'.
Bubble sort:    Array sorted, time:     49.106071 ms
Selection sort: Array sorted, time:     44.193520 ms
Counting sort:  Array sorted, time:      1.421377 ms
Quick sort:     Array sorted, time:      1.814473 ms
Std qsort:      Array sorted, time:      4.214441 ms
==1896915== 
==1896915== Events    : Ir
==1896915== Collected : 34347410
==1896915== 
==1896915== I   refs:      34,347,410
$ kcachegrind callgrind.out.1896915 
</pre>

kcachegrind:

![kcachegrind](https://github.com/EvgenSen/c-examples/blob/master/debug/callgrind.out.1896915.png)

