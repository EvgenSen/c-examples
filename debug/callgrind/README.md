## callgrind

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

![kcachegrind](https://github.com/EvgenSen/c-examples/blob/master/debug/callgrind/callgrind.out.1896915.png)
