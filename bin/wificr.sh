#!/bin/csh
sudo ifconfig wlan0 destroy
sudo ifconfig wlan0 create wlandev ath0 wlanmode hostap
sudo /etc/rc.d/hostapd onerestart
