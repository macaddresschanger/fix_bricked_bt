/*
 * 
 *  This project is a proof of concept example only.  
 *  The code demontrates sending commands to CSR bluetooth adapter (to change it's address) using libusb functions instead of hci calls.
 *  
 */

#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

#include <libusb-1.0/libusb.h>

void printhelp()
{
	cerr << "Usage:" << endl
		 << "       fix_bricked_bt VID PID BD_ADDR" << endl << endl
		 << "Arguments: " << endl
		 << "       VID, PID - USB device identifiers" << endl
		 << "       BD_ADDR  - New bluetooth address" << endl << endl
		 << "Example: " << endl
		 << "       fix_bricked_bt 0x0a12 0x0001 11:22:33:44:55:66 " << endl << endl;
}

struct bt_address
{
	union {
		unsigned long long u;
		char b[6];
	};
};

int main(int argc, char **argv)
{
	if (argc != 4)
	{
		cerr << "Invalid arguments." << endl;
		printhelp();
		return 1;
	}

	uint16_t vid, pid;
	vid = std::stoul(argv[1], nullptr, 16);
	pid = std::stoul(argv[2], nullptr, 16);

	string str{argv[3]};
	str.erase(std::remove(str.begin(), str.end(), ':'), str.end());
	bt_address addr{std::stoul(str, nullptr, 16)};

	libusb_context *ctx{nullptr};

	int r = libusb_init(&ctx);
	if (r < 0)
	{
		cerr << "Init Error. Code: " << r << endl;
		return 1;
	}

	libusb_set_debug(ctx, 3);

	libusb_device_handle *dev_handle = libusb_open_device_with_vid_pid(ctx, vid, pid);
	if (nullptr == dev_handle)
	{
		cerr << "Cannot open device" << endl;
		return 1;
	}
	else
	{
		cout << "Device Opened" << endl;
	}

	unsigned char cmd[] = {
		0x00, 0xfc, 0x19, 0xc2, 0x02, 0x00, 0x0c, 0x00, 0x11, 0x47,
		0x03, 0x70, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	int len = sizeof(cmd);

	cmd[27] = addr.b[5];
	cmd[26] = addr.b[4];
	cmd[24] = addr.b[3];
	cmd[20] = addr.b[2];
	cmd[23] = addr.b[1];
	cmd[22] = addr.b[0];

	if (libusb_kernel_driver_active(dev_handle, 0) == 1)
	{
		cout << "Kernel Driver Active" << endl;
		if (libusb_detach_kernel_driver(dev_handle, 0) == 0)
			cout << "Kernel Driver Detached!" << endl;
	}

	cout << "Writing Data..." << endl;

	r = libusb_control_transfer(
		dev_handle, // libusb_device_handle * dev_handle
		0x20,		// uint8_t  	          bmRequestType
		0,			// uint8_t  	          bRequest
		0,			// uint16_t  	          wValue
		0,			// uint16_t  	          wIndex
		cmd,		// unsigned char *  	  data
		len,		// uint16_t  	          wLength
		2000		// unsigned int  	      timeout
	);

	if (r == len)
	{
		cout << "Write Successful." << endl;
	}
	else
	{
		cout << "Write Error." << endl;
	}

	libusb_close(dev_handle);
	libusb_exit(ctx);

	return 0;
}
