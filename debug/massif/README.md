## massif

Output example:

<pre>
$ valgrind --tool=massif --time-unit=B ./example
==2347064== Massif, a heap profiler
==2347064== Copyright (C) 2003-2017, and GNU GPL'd, by Nicholas Nethercote
==2347064== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==2347064== Command: ./example
==2347064== 
==2347064== 
$ ms_print massif.out.2347064
--------------------------------------------------------------------------------
Command:            ./example
Massif arguments:   --time-unit=B
ms_print arguments: massif.out.2347064
--------------------------------------------------------------------------------


    KB
19.71^                                               ###                      
     |                                               #                        
     |                                               #  ::                    
     |                                               #  : :::                 
     |                                      :::::::::#  : :  ::               
     |                                      :        #  : :  : ::             
     |                                      :        #  : :  : : :::          
     |                                      :        #  : :  : : :  ::        
     |                            :::::::::::        #  : :  : : :  : :::     
     |                            :         :        #  : :  : : :  : :  ::   
     |                        :::::         :        #  : :  : : :  : :  : :: 
     |                     @@@:   :         :        #  : :  : : :  : :  : : @
     |                   ::@  :   :         :        #  : :  : : :  : :  : : @
     |                :::: @  :   :         :        #  : :  : : :  : :  : : @
     |              :::  : @  :   :         :        #  : :  : : :  : :  : : @
     |            ::: :  : @  :   :         :        #  : :  : : :  : :  : : @
     |         :::: : :  : @  :   :         :        #  : :  : : :  : :  : : @
     |       :::  : : :  : @  :   :         :        #  : :  : : :  : :  : : @
     |    :::: :  : : :  : @  :   :         :        #  : :  : : :  : :  : : @
     |  :::  : :  : : :  : @  :   :         :        #  : :  : : :  : :  : : @
   0 +----------------------------------------------------------------------->KB
     0                                                                   29.63

Number of snapshots: 25
 Detailed snapshots: [9, 14 (peak), 24]

--------------------------------------------------------------------------------
  n        time(B)         total(B)   useful-heap(B) extra-heap(B)    stacks(B)
--------------------------------------------------------------------------------
  0              0                0                0             0            0
  1          1,016            1,016            1,000            16            0
  2          2,032            2,032            2,000            32            0
  3          3,048            3,048            3,000            48            0
  4          4,064            4,064            4,000            64            0
  5          5,080            5,080            5,000            80            0
  6          6,096            6,096            6,000            96            0
  7          7,112            7,112            7,000           112            0
  8          8,128            8,128            8,000           128            0
  9          9,144            9,144            9,000           144            0
98.43% (9,000B) (heap allocation functions) malloc/new/new[], --alloc-fns, etc.
->98.43% (9,000B) 0x1091E5: main (example.c:20)
  
--------------------------------------------------------------------------------
  n        time(B)         total(B)   useful-heap(B) extra-heap(B)    stacks(B)
--------------------------------------------------------------------------------
 10         10,160           10,160           10,000           160            0
 11         12,168           12,168           12,000           168            0
 12         16,176           16,176           16,000           176            0
 13         20,184           20,184           20,000           184            0
 14         20,184           20,184           20,000           184            0
99.09% (20,000B) (heap allocation functions) malloc/new/new[], --alloc-fns, etc.
->49.54% (10,000B) 0x1091E5: main (example.c:20)
| 
->39.64% (8,000B) 0x10919A: g (example.c:5)
| ->19.82% (4,000B) 0x1091B4: f (example.c:11)
| | ->19.82% (4,000B) 0x109201: main (example.c:23)
| |   
| ->19.82% (4,000B) 0x109206: main (example.c:25)
|   
->09.91% (2,000B) 0x1091AF: f (example.c:10)
  ->09.91% (2,000B) 0x109201: main (example.c:23)
    
--------------------------------------------------------------------------------
  n        time(B)         total(B)   useful-heap(B) extra-heap(B)    stacks(B)
--------------------------------------------------------------------------------
 15         21,200           19,168           19,000           168            0
 16         22,216           18,152           18,000           152            0
 17         23,232           17,136           17,000           136            0
 18         24,248           16,120           16,000           120            0
 19         25,264           15,104           15,000           104            0
 20         26,280           14,088           14,000            88            0
 21         27,296           13,072           13,000            72            0
 22         28,312           12,056           12,000            56            0
 23         29,328           11,040           11,000            40            0
 24         30,344           10,024           10,000            24            0
99.76% (10,000B) (heap allocation functions) malloc/new/new[], --alloc-fns, etc.
->79.81% (8,000B) 0x10919A: g (example.c:5)
| ->39.90% (4,000B) 0x1091B4: f (example.c:11)
| | ->39.90% (4,000B) 0x109201: main (example.c:23)
| |   
| ->39.90% (4,000B) 0x109206: main (example.c:25)
|   
->19.95% (2,000B) 0x1091AF: f (example.c:10)
| ->19.95% (2,000B) 0x109201: main (example.c:23)
|   
->00.00% (0B) in 1+ places, all below ms_print's threshold (01.00%)

</pre>

## Massif-Visualizer

![Massif-Visualizer](https://github.com/EvgenSen/c-examples/blob/master/debug/massif/massif.out.2347064.png)

## References

https://valgrind.org/docs/manual/ms-manual.html