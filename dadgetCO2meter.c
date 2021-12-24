/*
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
*/

#define idVendor  0x04d9
#define idProduct 0xa052

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hidapi.h"

#define T       0x42
#define CO2     0x50

//////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
	int res;
	hid_device *handle;
	int type;
  unsigned char buf[8];

  if (argc > 1) {
    if (strcmp (argv[1], "-t") == 0) {
      type = T;
    }
    else {
      printf ("\ndadgetCO2meter is utility to get concentration of CO2 in air\n");
      printf ("or temperature of air by Dadget MT8057S Holtek USB-zyTemp device.\n");
      printf ("\nExamples of usage:\n");
      printf ("1). To get concentration of CO2 in ppm:\n# dadgetCO2meter\n");
      printf ("2). To get temperature of air in degC:\n# dadgetCO2meter -t\n\n");
      return 0;
    }
  }
  else
    type = CO2;

	// Initialize the hidapi library
	res = hid_init();
	if (res != 0) {
	  perror ("HID init");
	  return -1;
	}

	// Open the device using the idVendor, idProduct,
	// and optionally the Serial number.
	handle = hid_open (idVendor, idProduct, NULL);
	if (!handle) {
	  perror ("HID open");
	  return -2;
	}

  memset (buf, 0, 8);
  res = hid_send_feature_report (handle, buf, 8);
	if (res != 8) {
	  perror("HID send_feature_report");
	  return -3;
	}

  do
    res = hid_read (handle, buf, sizeof (buf));
  while (buf[0] != type);

  if (res > 0) {
    if (buf[4] == 0x0d && buf[5] == 0 && buf[6] == 0 && buf[7] == 0) {
      if (((buf[0] + buf[1] + buf[2]) & 0xff) == buf[3]) {
        if (type == CO2)
          printf ("%i\n", buf[1] * 256 + buf[2]);
        if (type == T)
          printf ("%.1f\n", ((float) buf[1] * 256 + buf[2]) / 16 - 273.15);
      }
      else perror ("Message checksum is not correct");
    }
    else perror ("Message tail is not correct");
	}
	else perror ("HID read");

	// Finalize the hidapi library
	res = hid_exit();
	if (res != 0) {
	  perror ("HID exit");
	  return -4;
	}

	return 0;
}

