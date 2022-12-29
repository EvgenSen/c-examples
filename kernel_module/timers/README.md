## Timer module

Output example:

<pre>
evgen@MyPC:~/projects/c-examples/kernel_module/timers$ sudo ./timer_base.sh 

  Module info:

filename:       /home/evgen/projects/c-examples/kernel_module/timers/timer_base.ko
version:        1.2
description:    Simple Timer module
author:         Evgeniy Sennikov <sennikov.work@ya.ru>
license:        GPL
srcversion:     29BB5BAA0E86A5B35ED2796
depends:        
retpoline:      Y
name:           timer_base
vermagic:       5.4.0-135-generic SMP mod_unload modversions

  Log info:

Dec 29 16:58:31 MyPC kernel: [70922.213183] timer_base: timer_base_init() HZ = 250; 1/HZ = 4 ms - minimal timer step
Dec 29 16:58:31 MyPC kernel: [70922.213184] timer_base: timer_base_init() msecs_to_jiffies(0) = 0
Dec 29 16:58:31 MyPC kernel: [70922.213184] timer_base: timer_base_init() msecs_to_jiffies(1) = 1
Dec 29 16:58:31 MyPC kernel: [70922.213185] timer_base: timer_base_init() msecs_to_jiffies(2) = 1
Dec 29 16:58:31 MyPC kernel: [70922.213185] timer_base: timer_base_init() msecs_to_jiffies(3) = 1
Dec 29 16:58:31 MyPC kernel: [70922.213186] timer_base: timer_base_init() msecs_to_jiffies(4) = 1
Dec 29 16:58:31 MyPC kernel: [70922.213186] timer_base: timer_base_init() msecs_to_jiffies(5) = 2
Dec 29 16:58:31 MyPC kernel: [70922.213186] timer_base: timer_base_init() msecs_to_jiffies(6) = 2
Dec 29 16:58:31 MyPC kernel: [70922.213187] timer_base: timer_base_init() msecs_to_jiffies(7) = 2
Dec 29 16:58:31 MyPC kernel: [70922.213187] timer_base: timer_base_init() msecs_to_jiffies(8) = 2
Dec 29 16:58:31 MyPC kernel: [70922.213188] timer_base: timer_base_init() msecs_to_jiffies(9) = 3
Dec 29 16:58:31 MyPC kernel: [70922.213188] timer_base: timer_base_init() msecs_to_jiffies(10) = 3
Dec 29 16:58:31 MyPC kernel: [70922.213189] timer_base: timer_base_init() msecs_to_jiffies(100) = 25
Dec 29 16:58:31 MyPC kernel: [70922.314165] timer_base: test_timer_func() started, counter = 5, data = test_data_for_timer_cb
Dec 29 16:58:31 MyPC kernel: [70922.378191] timer_base: test_timer_func() started, counter = 4, data = test_data_for_timer_cb
Dec 29 16:58:31 MyPC kernel: [70922.438171] timer_base: test_timer_func() started, counter = 3, data = test_data_for_timer_cb
Dec 29 16:58:31 MyPC kernel: [70922.482188] timer_base: test_timer_func() started, counter = 2, data = test_data_for_timer_cb
Dec 29 16:58:31 MyPC kernel: [70922.518191] timer_base: test_timer_func() started, counter = 1, data = test_data_for_timer_cb
Dec 29 16:58:31 MyPC kernel: [70922.542190] timer_base: test_timer_func() started, counter = 0, data = test_data_for_timer_cb
Dec 29 16:58:32 MyPC kernel: [70923.219590] timer_base: timer_base_cleanup() started
</pre>
