[![megabas-rpi](res/sequent.jpg)](https://sequentmicrosystems.com/index.php?route=product/product&path=33&product_id=65)

# Modbus

The [Building](https://sequentmicrosystems.com/index.php?route=product/product&path=33&product_id=65) Automation I/O Expansion cards for Raspberry Pi can be accessed thru Modbus RTU protocol over RS-485 port.
You can set-up the RS-485 port with **megabas** command.

Example:
```bash
~$ megabas 0 rs485wr 1 9600 1 0 1
```
Set Modbus RTU , Baudrate: 9600bps, 1 Stop Bit,  parity: None, slave address offset: 1
```bash
~$ megabas -h rs485wr
```
display the full set of options

## Slave Address
The slave address is add with the "stack level" jumpers. For example the jumpers configuration for stack level 1  (one jumper in position ID1) slave address offset to 1 corespond to slave address 2.

## Modbus object types
All modbus RTU object type with standard addresses are implemented : Coils, Discrete Inputs, Input registers, Holding registers.

### Coils

Acces level Read/Write, Size 1 bit

| Device function | Register Address | Modbus Address |
| --- | --- | --- |
| TRIAC_1 | 0001 | 0x00 |
| TRIAC_2 | 0002 | 0x01 |
| TRIAC_3 | 0003 | 0x02 |
| TRIAC_4 | 0004 | 0x03 |

### Discrete Inputs

Access level Read Only, Size 1 bit

| Device function | Register Address | Modbus Address |
| --- | --- | --- |
| DRY_CONTACT_1 | 10001 | 0x00 |
| DRY_CONTACT_2 | 10002 | 0x01 |
| DRY_CONTACT_3 | 10003 | 0x02 |
| DRY_CONTACT_4 | 10004 | 0x03 |
| DRY_CONTACT_5 | 10005 | 0x04 |
| DRY_CONTACT_6 | 10006 | 0x05 |
| DRY_CONTACT_7 | 10007 | 0x06 |
| DRY_CONTACT_8 | 10008 | 0x07 |

### Input registers

Access level Read Only, Size 16 bits

| Device function | Register Address | Modbus Address | Measurement Unit |
| --- | --- | --- | --- |
| 0-10V_IN_1 | 30001 | 0x00 | mV |
| 0-10V_IN_2 | 30002 | 0x01 | mV |
| 0-10V_IN_3 | 30003 | 0x02 | mV |
| 0-10V_IN_4 | 30004 | 0x03 | mV |
| 0-10V_IN_5 | 30005 | 0x04 | mV |
| 0-10V_IN_6 | 30006 | 0x05 | mV |
| 0-10V_IN_7 | 30007 | 0x06 | mV |
| 0-10V_IN_8 | 30008 | 0x07 | mV |
| 1K_R_IN_1 | 30009 | 0x08 | ohm |
| 1K_R_IN_2 | 30010 | 0x09 | ohm |
| 1K_R_IN_3 | 30011 | 0x0a | ohm |
| 1K_R_IN_4 | 30012 | 0x0b | ohm |
| 1K_R_IN_5 | 30013 | 0x0c | ohm |
| 1K_R_IN_6 | 30014 | 0x0d | ohm |
| 1K_R_IN_7 | 30015 | 0x0e | ohm |
| 1K_R_IN_8 | 30016 | 0x0f | ohm |
| 10K_R_IN_1 | 30017 | 0x10 | ohm |
| 10K_R_IN_2 | 30018 | 0x11 | ohm |
| 10K_R_IN_3 | 30019 | 0x12 | ohm |
| 10K_R_IN_4 | 30020 | 0x13 | ohm |
| 10K_R_IN_5 | 30021 | 0x14 | ohm |
| 10K_R_IN_6 | 30022 | 0x15 | ohm |
| 10K_R_IN_7 | 30023 | 0x16 | ohm |
| 10K_R_IN_8 | 30024 | 0x17 | ohm |

### Holding registers

Access level Read/Write, Size 16 bits

| Device function | Register Address | Modbus Address | Measurement Unit |
| --- | --- | --- | --- |
| 0-10V_OUT_1 | 40001 | 0x00 | mV |
| 0-10V_OUT_2 | 40002 | 0x01 | mV |
| 0-10V_OUT_3 | 40003 | 0x02 | mV |
| 0-10V_OUT_4 | 40004 | 0x03 | mV |



## Function codes implemented

* Read Coils (0x01)
* Read Discrete Inputs (0x02)
* Read Holding Registers (0x03)
* Read Input Registers (0x04)
* Write Single Coil (0x05)
* Write Single Register (0x06)
* Write Multiple Coils (0x0f)
* Write Multiple registers (0x10)
