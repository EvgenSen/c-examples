## Interaction proc module

Output example:

<pre>
evgen@MyPC:~/projects/c-examples/kernel_module$ sudo ./interaction_proc.sh 

  Module info:

filename:       /home/evgen/projects/c-examples/kernel_module/interaction_proc.ko
version:        1.0
description:    Simple proc Hello world module
author:         Evgeniy Sennikov <sennikov.work@ya.ru>
license:        GPL
srcversion:     3B174B4DCCCA0275147CC13
depends:        
retpoline:      Y
vermagic:       4.4.0-146-generic SMP mod_unload modversions 686 retpoline 

  Proc output:

Hello world from proc

  Log info:

Sep  2 11:29:57 MyPC kernel: [519947.685545] hello_world: loading out-of-tree module taints kernel.
Sep  2 11:29:57 MyPC kernel: [519947.685571] hello_world: module verification failed: signature and/or required key missing - tainting kernel
Sep  2 11:29:57 MyPC kernel: [519947.685931]  #### hello_world module started
Sep  2 11:29:57 MyPC kernel: [519947.686529]  #### hello_world module stopped
Sep  2 11:30:43 MyPC kernel: [519993.606153]  #### hello_world module started
Sep  2 11:30:43 MyPC kernel: [519993.607805]  #### hello_world module stopped
Sep  2 11:30:50 MyPC kernel: [520000.796033]  #### hello_world module started
Sep  2 11:30:50 MyPC kernel: [520000.797215]  #### hello_world module stopped
Sep  2 11:34:58 MyPC kernel: [520248.916848] interaction_proc: Proc file was opened
Sep  2 11:34:58 MyPC kernel: [520248.917097] interaction_proc: You write to proc file "test msg for kernel"

</pre>
