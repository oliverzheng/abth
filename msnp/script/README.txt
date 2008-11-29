To use:

1) Open Cygwin
2) ./WinDump.exe -D
   Find the network interface you want to use. Remember the leading numeral. Mine was 3.
3) SSH into oliverzheng.com, and launch /home/oliverzheng/services/mixer
4) ./WinDump.exe -i <NETWORK_INTERFACE_#> -p -A -l -n -S -t -e -X tcp port 1863 | perl find_wait_together.pl <NETWORK_INTERFACE_#>
   e.g. Mine was: ./WinDump.exe -i 3 -p -A -l -n -S -t -e -X tcp port 1863 | perl find_wait_together.pl 3
