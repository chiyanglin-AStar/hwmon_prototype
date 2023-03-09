# hwmon module protoype
ref : https://github.com/cengiz-io/hwmon-module-test

This module tries to build an absolute minimum `hwmon` device which abstracts `sysfs` attribute creation hassle to API.

sudo apt install build-essential

make

## install module

sudo insmod hm_proto.ko && lsmod | grep h_mon_p || echo "Failed to insmod"

or

sudo insmod hm_proto.ko

and to find the following path :

/sys/bus/platform/devices/h_mon_p-dev
/sys/bus/platform/drivers/h_mon_p-dev
....

/sys/devices/platform/h_mon_p-dev
......

## test module

cat /sys/module/hw_probe/drivers/platform\:h_mon_p-dev/h_mon_p-dev/hwmon/hwmon3/temp1_emergency

## uninstall module

sudo rmmod h_mon_p && lsmod | grep h_mon_p && echo "Failed to rmmod"

```
