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

## Network module

Output example:

<pre>
evgen@MyPC:~/projects/c-examples/kernel_module$ sudo ./network_mod.sh 

  Module info:

filename:       /home/evgen/projects/c-examples/kernel_module/network_mod.ko
version:        1.0
description:    Simple network module
author:         Evgeniy Sennikov <sennikov.work@ya.ru>
license:        GPL
srcversion:     D0F0F064FE47EA519428073
depends:        
retpoline:      Y
vermagic:       4.4.0-148-generic SMP mod_unload modversions 

  Ifconfig dummy0:

dummy0    Link encap:Ethernet  HWaddr 00:01:02:03:04:05  
          BROADCAST MULTICAST  MTU:1500  Metric:1
          RX packets:0 errors:0 dropped:0 overruns:0 frame:0
          TX packets:0 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:1000 
          RX bytes:0 (0.0 B)  TX bytes:0 (0.0 B)


  Set MAC address 06:05:04:03:02:01 for interface dummy0

  Set IP address 192.168.123.123 for interface dummy0

  Ifconfig dummy0:

dummy0    Link encap:Ethernet  HWaddr 06:05:04:03:02:01  
          inet addr:192.168.123.123  Bcast:192.168.123.255  Mask:255.255.255.0
          inet6 addr: fe80::405:4ff:fe03:201/64 Scope:Link
          UP BROADCAST RUNNING MULTICAST  MTU:1500  Metric:1
          RX packets:0 errors:0 dropped:0 overruns:0 frame:0
          TX packets:0 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:1000 
          RX bytes:0 (0.0 B)  TX bytes:0 (0.0 B)

  Ping interface:

PING 192.168.123.123 (192.168.123.123) 56(84) bytes of data.
64 bytes from 192.168.123.123: icmp_seq=1 ttl=64 time=0.021 ms
64 bytes from 192.168.123.123: icmp_seq=2 ttl=64 time=0.010 ms
64 bytes from 192.168.123.123: icmp_seq=3 ttl=64 time=0.019 ms
64 bytes from 192.168.123.123: icmp_seq=4 ttl=64 time=0.035 ms

--- 192.168.123.123 ping statistics ---
4 packets transmitted, 4 received, 0% packet loss, time 2997ms
rtt min/avg/max/mdev = 0.010/0.021/0.035/0.009 ms

  Log info:

Feb 13 13:26:53 MyPC kernel: [693593.917486] usb 3-1.3: usbfs: process 11862 (update) did not claim interface 0 before use
Feb 13 13:26:53 MyPC kernel: [693594.307975] usb 3-1.3: usbfs: process 11864 (update) did not claim interface 0 before use
Feb 13 13:26:53 MyPC kernel: [693594.425619] usb 3-1.3: usbfs: process 11864 (update) did not claim interface 0 before use
Feb 13 13:28:23 MyPC kernel: [693684.479181] usb 3-1.3: USB disconnect, device number 105
Feb 13 13:42:24 MyPC kernel: [694525.287798] network_mod: Init
Feb 13 13:42:24 MyPC kernel: [694525.287803] network_mod: Setup
Feb 13 13:42:24 MyPC kernel: [694525.287929] network_mod: Init dummy0 - Ok
Feb 13 13:42:24 MyPC kernel: [694525.305719] network_mod: Open dummy0
Feb 13 13:42:27 MyPC kernel: [694528.315451] network_mod: Exit
Feb 13 13:42:27 MyPC kernel: [694528.315487] network_mod: Close dummy0

</pre>
