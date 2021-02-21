## Interaction proc module

Output example:

<pre>
evgen@MyPC:~/projects/c-examples/kernel_module/interaction$ sudo ./interaction_proc.sh 

  Module info:

filename:       /home/evgen/projects/c-examples/kernel_module/interaction/interaction_proc.ko
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

## Interaction ioctl module

Output example:

<pre>
evgen@MyPC:~/projects/c-examples/kernel_module/interaction$ sudo ./interaction_ioctl.sh 

  Module info:

filename:       /home/evgen/all_projetcs/c-examples/kernel_module/interaction/interaction_ioctl.ko
version:        1.0
description:    Basic ioctl module
author:         Evgeniy Sennikov <sennikov.work@ya.ru>
license:        GPL
srcversion:     55C4AAB54A327216EE94C0B
depends:        
retpoline:      Y
vermagic:       4.4.0-144-generic SMP mod_unload modversions 686 retpoline 

  Start userspace_ioctl:

userspace_ioctl.c: 1 set flag=7, buff="test_msg_for_kernel"
userspace_ioctl.c: 2 nil flag=0, buff=""
userspace_ioctl.c: 3 get flag=7, buff="test_msg_for_kernel"

  Log info:

Feb 21 19:36:33 MyPC kernel: [ 1484.097354] usb 3-2: Product: USB2.0 Hub
Feb 21 19:36:33 MyPC kernel: [ 1484.098069] hub 3-2:1.0: USB hub found
Feb 21 19:36:33 MyPC kernel: [ 1484.098319] hub 3-2:1.0: 4 ports detected
Feb 21 19:36:58 MyPC kernel: [ 1509.853334] interaction_ioctl: Regiseter character device ok
Feb 21 19:36:58 MyPC kernel: [ 1509.855614] interaction_ioctl: inter_mod_open() started
Feb 21 19:36:58 MyPC kernel: [ 1509.855631] interaction_ioctl: inter_mod_ioctl() started
Feb 21 19:36:58 MyPC kernel: [ 1509.855633] interaction_ioctl: Set cmd flag=7, buffer="test_msg_for_kernel", ret = 0
Feb 21 19:36:58 MyPC kernel: [ 1509.855641] interaction_ioctl: inter_mod_ioctl() started
Feb 21 19:36:58 MyPC kernel: [ 1509.855643] interaction_ioctl: Get cmd flag=7, buffer="test_msg_for_kernel", ret = 0
Feb 21 19:36:58 MyPC kernel: [ 1509.855648] interaction_ioctl: inter_mod_release() started

</pre>
