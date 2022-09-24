## perf

Program start example.
While program running, the number in timeout.txt file changes from 100000 to 1000

<pre>
$ echo 100000 > timeout.txt
$  ./perf_test &
[1] 1290067
$  ps aux | grep perf     
evgen    1290067 58.4  0.0   2488   508 pts/0    RN   18:29   0:02 ./perf_test
$  sudo perf record -g -p 1290067
^C[ perf record: Woken up 10 times to write data ]
[ perf record: Captured and wrote 2,871 MB perf.data (29788 samples) ]

$ sudo kill 1290067                    
[1]  + 1290067 terminated  ./perf_test
$ 
$ sudo perf script > perf_script.output
</pre>

## flamescope

![flamescope_1](https://github.com/EvgenSen/c-examples/blob/master/debug/perf/flamescope_1.png)

![flamescope_2](https://github.com/EvgenSen/c-examples/blob/master/debug/perf/flamescope_2.png)

## References

https://www.brendangregg.com/perf.html

https://github.com/Netflix/flamescope
