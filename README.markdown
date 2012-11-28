# Call Backup and Restore for Meego

## Installation

Deb file can be downloaded from <https://github.com/wwwjfy/MeegoCallBackupRestore/downloads>.

run on device

    dpkg -i callbackuprestore_0.1_armel.deb

## Format

It uses psudo-csv format without header.

	local_phone_number,remote_phone_number,direction,start_date,end_date

- local\_phone\_number: the phone number of this device
- remote\_phone\_number: the phone number the call is from/to
- direction: 0 for outbound, 1 for inbound, 2 for missing
- start\_date: the format is yyyy-MM-dd hh:mm:ss
- end\_date: the format is yyyy-MM-dd hh:mm:ss

## Functionalities

### Import

Usage:

    callbackuprestore import [filename]

### Export

Usage:

    callbackuprestore export [filename]
