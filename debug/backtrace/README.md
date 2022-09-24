## backtrace

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
