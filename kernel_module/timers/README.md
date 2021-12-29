## Hello world module

Output example:

<pre>
evgen@MyPC:~/projects/c-examples/kernel_module$ sudo ./hello_world.sh 

  Module info:

filename:       /home/evgen/projects/c-examples/kernel_module/hello_world.ko
version:        1.0
description:    Simple Hello world module
author:         Evgeniy Sennikov <sennikov.work@ya.ru>
license:        GPL
srcversion:     300F960BB303D5E4BDA0216
depends:        
retpoline:      Y
vermagic:       4.4.0-146-generic SMP mod_unload modversions 686 retpoline 

  Log info:

Sep  2 09:44:42 MyPC kernel: [513632.863872] sr 10:0:0:0: Attached scsi CD-ROM sr0
Sep  2 09:44:42 MyPC kernel: [513632.867743] sr 10:0:0:0: Attached scsi generic sg1 type 5
Sep  2 09:44:42 MyPC kernel: [513633.417475] ISO 9660 Extensions: Microsoft Joliet Level 1
Sep  2 09:44:42 MyPC kernel: [513633.417881] ISOFS: changing to secondary root
Sep  2 11:29:57 MyPC kernel: [519947.685545] hello_world: loading out-of-tree module taints kernel.
Sep  2 11:29:57 MyPC kernel: [519947.685571] hello_world: module verification failed: signature and/or required key missing - tainting kernel
Sep  2 11:29:57 MyPC kernel: [519947.685931]  #### hello_world module started
Sep  2 11:29:57 MyPC kernel: [519947.686529]  #### hello_world module stopped

</pre>

evgen@evgen-work:/media/evgen/os_x86/home/evgen/all_second/c-examples/kernel_module/timers$ sudo ./timer_base.sh 

  Module info:

filename:       /media/evgen/os_x86/home/evgen/all_second/c-examples/kernel_module/timers/timer_base.ko
version:        1.0
description:    Simple Timer module
author:         Evgeniy Sennikov <sennikov.work@ya.ru>
license:        GPL
srcversion:     B3FD7E7E2D19A52C4DBA477
depends:        
retpoline:      Y
vermagic:       4.4.0-148-generic SMP mod_unload modversions 

  Log info:

Nov 23 17:04:09 evgen-work kernel: [684553.620733] timer_base: timer_base_init() started
Nov 23 17:04:09 evgen-work kernel: [684553.620737] timer_base: timer_base_init() HZ = 250; 1/HZ = 4 ms - minimal timer step
Nov 23 17:04:09 evgen-work kernel: [684553.620739] timer_base: timer_base_init() msecs_to_jiffies(1) = 1
Nov 23 17:04:09 evgen-work kernel: [684553.620740] timer_base: timer_base_init() msecs_to_jiffies(2) = 1
Nov 23 17:04:09 evgen-work kernel: [684553.620742] timer_base: timer_base_init() msecs_to_jiffies(3) = 1
Nov 23 17:04:09 evgen-work kernel: [684553.620743] timer_base: timer_base_init() msecs_to_jiffies(4) = 1
Nov 23 17:04:09 evgen-work kernel: [684553.620744] timer_base: timer_base_init() msecs_to_jiffies(5) = 2
Nov 23 17:04:09 evgen-work kernel: [684553.620745] timer_base: timer_base_init() msecs_to_jiffies(6) = 2
Nov 23 17:04:09 evgen-work kernel: [684553.620747] timer_base: timer_base_init() msecs_to_jiffies(7) = 2
Nov 23 17:04:09 evgen-work kernel: [684553.620748] timer_base: timer_base_init() msecs_to_jiffies(8) = 2
Nov 23 17:04:09 evgen-work kernel: [684553.620749] timer_base: timer_base_init() msecs_to_jiffies(9) = 3
Nov 23 17:04:09 evgen-work kernel: [684553.620751] timer_base: timer_base_init() msecs_to_jiffies(10) = 3
Nov 23 17:04:09 evgen-work kernel: [684553.620752] timer_base: timer_base_init() msecs_to_jiffies(100) = 25
Nov 23 17:04:09 evgen-work kernel: [684553.720216] timer_base: test_timer_func() started, counter = 5
Nov 23 17:04:09 evgen-work kernel: [684553.724215] timer_base: test_timer_func() started, counter = 4
Nov 23 17:04:09 evgen-work kernel: [684553.728213] timer_base: test_timer_func() started, counter = 3
Nov 23 17:04:09 evgen-work kernel: [684553.732214] timer_base: test_timer_func() started, counter = 2
Nov 23 17:04:09 evgen-work kernel: [684553.736212] timer_base: test_timer_func() started, counter = 1
Nov 23 17:04:09 evgen-work kernel: [684553.740212] timer_base: test_timer_func() started, counter = 0
Nov 23 17:04:10 evgen-work kernel: [684554.622255] timer_base: timer_base_cleanup() started
