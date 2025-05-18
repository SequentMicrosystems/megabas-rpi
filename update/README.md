# update

This is the [Sequent Microsystems](https://www.sequentmicrosystems.com) Building automation systems board firmware update tool.

## Usage

```bash
~$ git clone https://github.com/SequentMicrosystems/megabas-rpi.git
~$ cd megabas-rpi/update/
~/megabas-rpi/update$ ./update 0
```

For Ubuntu users, do the following:
```bash
~$ git clone https://github.com/SequentMicrosystems/megabas-rpi.git
~$ cd megabas-rpi/update/ubuntu/
~/megabas-rpi/update/ubuntu$ sudo ./update64 0
```

For 64-bit Linux users, do the following:
```bash
~$ git clone https://github.com/SequentMicrosystems/megabas-rpi.git
~$ cd megabas-rpi/update/
~/megabas-rpi/update$ sudo ./update64 0
```

If you clone the repository, skip the first step. 
The command will download the newest firmware version from our server and write it to the board.
The stack level of the board must be provided as a parameter. 
Make sure no one (Python scripts, or sell scripts, or Node-RED flow) tries to access the card during the update process.
During firmware update, we strongly recommend disconnecting all outputs from the board since they can change state unpredictably.


