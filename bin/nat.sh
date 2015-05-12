#!/bin/csh
sudo ipfw -q flush


#sudo ipfw nat 1 config log if em0 reset same_ports
#sudo ipfw add 120 nat 1 ip from 192.168.1.0/24 to any via em0
#sudo ipfw add 121 nat 1 ip from any to 192.168.1.53 via em0



sudo ipfw add 1000 deny ip from any to 192.168.1.0/24 in via wlan0
sudo ipfw nat 1 config ip 10.23.6.11 reset
sudo ipfw add 2000 nat 1 all from any to any
sudo ipfw add 3000 allow ip from any to any
