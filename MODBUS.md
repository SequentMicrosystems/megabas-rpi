[![megabas-rpi](res/sequent.jpg)](https://sequentmicrosystems.com)

# Modbus

The [Building](https://sequentmicrosystems.com/product/raspberry-pi-building-automation/) Automation I/O Expansion cards for Raspberry Pi can be accessed thru Modbus RTU protocol over RS-485 port.
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
The slave address is add with the "stack level" jumpers. For example the jumpers configuration for stack level 1  (one jumper in position ID0) slave address offset to 1 corespond to slave address 2.

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
| DRY_CONTACT_RISING_COUNT_ENABLE_CH1 | 0005 | 0x04 |
| DRY_CONTACT_RISING_COUNT_ENABLE_CH2 | 0006 | 0x05 |
| DRY_CONTACT_RISING_COUNT_ENABLE_CH3 | 0007 | 0x06 |
| DRY_CONTACT_RISING_COUNT_ENABLE_CH4 | 0008 | 0x07 |
| DRY_CONTACT_RISING_COUNT_ENABLE_CH5 | 0009 | 0x08 |
| DRY_CONTACT_RISING_COUNT_ENABLE_CH6 | 0010 | 0x09 |
| DRY_CONTACT_RISING_COUNT_ENABLE_CH7 | 0011 | 0x0a |
| DRY_CONTACT_RISING_COUNT_ENABLE_CH8 | 0012 | 0x0b |
| DRY_CONTACT_FALLING_COUNT_ENABLE_CH1 | 0013 | 0x0c |
| DRY_CONTACT_FALLING_COUNT_ENABLE_CH2 | 0014 | 0x0d |
| DRY_CONTACT_FALLING_COUNT_ENABLE_CH3 | 0015 | 0x0e |
| DRY_CONTACT_FALLING_COUNT_ENABLE_CH4 | 0016 | 0x0f |
| DRY_CONTACT_FALLING_COUNT_ENABLE_CH5 | 0017 | 0x10 |
| DRY_CONTACT_FALLING_COUNT_ENABLE_CH6 | 0018 | 0x11 |
| DRY_CONTACT_FALLING_COUNT_ENABLE_CH7 | 0019 | 0x12 |
| DRY_CONTACT_FALLING_COUNT_ENABLE_CH8 | 0020 | 0x13 |

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
| DRY_CONTACT_EDGE_COUNTER_CH1 | 30025 | 0x18 | - |
| DRY_CONTACT_EDGE_COUNTER_CH2 | 30026 | 0x19 | - |
| DRY_CONTACT_EDGE_COUNTER_CH3 | 30027 | 0x1a | - |
| DRY_CONTACT_EDGE_COUNTER_CH4 | 30028 | 0x1b | - |
| DRY_CONTACT_EDGE_COUNTER_CH5 | 30029 | 0x1c | - |
| DRY_CONTACT_EDGE_COUNTER_CH6 | 30030 | 0x1d | - |
| DRY_CONTACT_EDGE_COUNTER_CH7 | 30031 | 0x1e | - |
| DRY_CONTACT_EDGE_COUNTER_CH8 | 30032 | 0x1f | - |


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
