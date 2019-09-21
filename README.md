
This project is a proof of concept example only.  
The code demontrates sending commands to CSR bluetooth adapter (to change it's address) using libusb functions instead of hci calls.


Usage:
       fix_bricked_bt VID PID BD_ADDR

Arguments:
       VID, PID - USB device identifiers
       BD_ADDR  - New bluetooth address

Example:
       fix_bricked_bt 0x0a12 0x0001 11:22:33:44:55:66



Building
Run make to build the program. libusb-1.0 headers and libraries required to be installed on your system (debian/ubuntu - "libusb-1.0-0-dev" package)