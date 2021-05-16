## Network module

Output example:

<pre>
evgen@MyPC:~/projects/c-examples/kernel_module/network$ make
make[1]: Вход в каталог `/usr/src/linux-headers-4.4.0-148-generic'
  CC [M]  /home/evgen/projects/c-examples/kernel_module/network/network_base.o
  Building modules, stage 2.
  MODPOST 1 modules
  CC      /home/evgen/projects/c-examples/kernel_module/network/network_base.mod.o
  LD [M]  /home/evgen/projects/c-examples/kernel_module/network/network_base.ko
make[1]: Выход из каталога `/usr/src/linux-headers-4.4.0-148-generic'

evgen@MyPC:~/projects/c-examples/kernel_module/network$ sudo ./network_base.sh

  Module info:

filename:       /home/evgen/projects/c-examples/kernel_module/network/network_base.ko
version:        1.1
description:    Simple network module
author:         Evgeniy Sennikov <sennikov.work@ya.ru>
license:        GPL
srcversion:     A13063E239745459AD350DD
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
Feb 13 13:42:24 MyPC kernel: [694525.287798] network_base: Init
Feb 13 13:42:24 MyPC kernel: [694525.287803] network_base: Setup
Feb 13 13:42:24 MyPC kernel: [694525.287929] network_base: Init dummy0 - Ok
Feb 13 13:42:24 MyPC kernel: [694525.305719] network_base: Open dummy0
Feb 13 13:42:27 MyPC kernel: [694528.315451] network_base: Exit
Feb 13 13:42:27 MyPC kernel: [694528.315487] network_base: Close dummy0 (Private data: "Data from open() func")

</pre>
