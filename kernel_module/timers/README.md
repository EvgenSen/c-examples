## Timer module

Output example:

<pre>
evgen@MyPC:~/projects/c-examples/kernel_module/timers$ sudo ./timer_base.sh 

  Module info:

filename:       /home/evgen/projects/c-examples/kernel_module/timers/timer_base.ko
version:        1.1
description:    Simple Timer module
author:         Evgeniy Sennikov <sennikov.work@ya.ru>
license:        GPL
srcversion:     A8AEB233E92FBCEFCA8E56D
depends:        
retpoline:      Y
name:           timer_base
vermagic:       5.13.0-30-generic SMP mod_unload modversions 

  Log info:

Apr  2 15:03:54 MyPC kernel: [  556.618707] timer_base: timer_base_init() HZ = 250; 1/HZ = 4 ms - minimal timer step
Apr  2 15:03:54 MyPC kernel: [  556.618709] timer_base: timer_base_init() msecs_to_jiffies(0) = 0
Apr  2 15:03:54 MyPC kernel: [  556.618710] timer_base: timer_base_init() msecs_to_jiffies(1) = 1
Apr  2 15:03:54 MyPC kernel: [  556.618711] timer_base: timer_base_init() msecs_to_jiffies(2) = 1
Apr  2 15:03:54 MyPC kernel: [  556.618712] timer_base: timer_base_init() msecs_to_jiffies(3) = 1
Apr  2 15:03:54 MyPC kernel: [  556.618713] timer_base: timer_base_init() msecs_to_jiffies(4) = 1
Apr  2 15:03:54 MyPC kernel: [  556.618714] timer_base: timer_base_init() msecs_to_jiffies(5) = 2
Apr  2 15:03:54 MyPC kernel: [  556.618715] timer_base: timer_base_init() msecs_to_jiffies(6) = 2
Apr  2 15:03:54 MyPC kernel: [  556.618716] timer_base: timer_base_init() msecs_to_jiffies(7) = 2
Apr  2 15:03:54 MyPC kernel: [  556.618717] timer_base: timer_base_init() msecs_to_jiffies(8) = 2
Apr  2 15:03:54 MyPC kernel: [  556.618718] timer_base: timer_base_init() msecs_to_jiffies(9) = 3
Apr  2 15:03:54 MyPC kernel: [  556.618719] timer_base: timer_base_init() msecs_to_jiffies(10) = 3
Apr  2 15:03:54 MyPC kernel: [  556.618720] timer_base: timer_base_init() msecs_to_jiffies(100) = 25
Apr  2 15:03:54 MyPC kernel: [  556.720132] timer_base: test_timer_func() started, counter = 5
Apr  2 15:03:54 MyPC kernel: [  556.784175] timer_base: test_timer_func() started, counter = 4
Apr  2 15:03:54 MyPC kernel: [  556.840115] timer_base: test_timer_func() started, counter = 3
Apr  2 15:03:54 MyPC kernel: [  556.884119] timer_base: test_timer_func() started, counter = 2
Apr  2 15:03:54 MyPC kernel: [  556.920146] timer_base: test_timer_func() started, counter = 1
Apr  2 15:03:54 MyPC kernel: [  556.944104] timer_base: test_timer_func() started, counter = 0
Apr  2 15:03:55 MyPC kernel: [  557.623288] timer_base: timer_base_cleanup() started
</pre>
