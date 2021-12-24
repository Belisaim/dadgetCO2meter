# dadgetCO2meter

dadgetCO2meter is utility to get concentration of CO2 in air
or temperature of air by Dadget MT8057S Holtek USB-zyTemp device.

Examples of usage:
1). To get concentration of CO2 in ppm:
# dadgetCO2meter
2). To get temperature of air in degC:
# dadgetCO2meter -t

It was tested on Ubuntu 20.04.3 LTS.
Sources of information:
https://github.com/vfilimonov/co2meter/blob/master/co2meter/co2meter.py
And /usr/share/doc/libhidapi-dev/README.md.gz in the pack of libhidapi-dev.

Before compiling:
# apt install libhidapi-dev libudev-dev

Insert CO2 meter to USB and give the command:
# dmesg
...
[ 6741.843740] usb 3-6: new low-speed USB device number 5 using xhci_hcd
[ 6742.002696] usb 3-6: New USB device found, idVendor=04d9, idProduct=a052, bcdDevice= 2.00
[ 6742.002702] usb 3-6: New USB device strings: Mfr=1, Product=2, SerialNumber=3
[ 6742.002705] usb 3-6: Product: USB-zyTemp
[ 6742.002708] usb 3-6: Manufacturer: Holtek
[ 6742.002711] usb 3-6: SerialNumber: 2.00
[ 6742.011635] hid-generic 0003:04D9:A052.0002: hiddev0,hidraw1: USB HID v1.10 Device [Holtek USB-zyTemp] on usb-0000:00:14.0-6/input0

or the command:
$ lsusb
...
Bus 003 Device 005: ID 04d9:a052 Holtek Semiconductor, Inc. USB-zyTemp
...

We must find idVendor and idProduct and insert them to our program.
Then to compile:
$ gcc dadgetCO2meter.c /usr/lib/x86_64-linux-gnu/libhidapi-hidraw.a -ludev -I/usr/include/hidapi -o dadgetCO2meter

Enjoy!
