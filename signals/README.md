## signals

Output example:

<pre>
evgen@MyPC:~/projects/c-examples/signals$ ./start_example.sh 
main(): Send signal SIGUSR1 (10)
get_sigusr1(): Recv signal 10
main(): Send signal SIGUSR2 (12)
get_sigusr2(): Recv signal 12
Send SIGUSR1
get_sigusr1(): Recv signal 10
Send SIGUSR2
get_sigusr2(): Recv signal 12
Send SIGTERM
get_sigterm(): Recv signal 15
Send SIGKILL
./start_example.sh: строка 28:   886 Убито              ./signals
</pre>
