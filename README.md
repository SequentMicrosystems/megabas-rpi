[![megabas-rpi](res/sequent.jpg)](https://www.sequentmicrosystems.com)

# megabas-rpi

This is the command to control [Building Automation Stackable Card for Raspberry Pi](https://sequentmicrosystems.com/product/raspberry-pi-building-automation/)

![MEGA-BAS](res/megabas.jpg)

## Setup

Don't forget to enable I2C communication:
```bash
~$ sudo raspi-config
```

## Usage

```bash
~$ git clone https://github.com/SequentMicrosystems/megabas-rpi.git
~$ cd megabas-rpi/
~/megabas-rpi$ sudo make install
```

Now you can access all the functions of the BAS board through the command "megabas". Use -h option for help:
```bash
~$ megabas -h
```

If you clone the repository any update can be made with the following commands:

```bash
~$ cd megabas-rpi/  
~/megabas-rpi$ git pull
~/megabas-rpi$ sudo make install
```  
[Python Library](python/README.md)

[Firmware update instructions](update/README.md).

[Node-RED](https://github.com/SequentMicrosystems/megabas-rpi/tree/master/node-red-contrib-sm-bas)

[CODESYS Driver](https://github.com/SequentMicrosystems/megabas-rpi/tree/master/CODESYS)

The board can act as Modbus RTU slave device, checkout [modbus instructions](MODBUS.md)
