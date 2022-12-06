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

| Device function | Register Address | Description | Measurement Unit |
| --- | --- | --- | --- |
| 0-10V_IN_1 | 30001 | 0 - 10V input channel 1 | mV |
| 0-10V_IN_2 | 30002 | 0 - 10V input channel 2 | mV |
| 0-10V_IN_3 | 30003 | 0 - 10V input channel 3 | mV |
| 0-10V_IN_4 | 30004 | 0 - 10V input channel 4 | mV |
| 0-10V_IN_5 | 30005 | 0 - 10V input channel 5 | mV |
| 0-10V_IN_6 | 30006 | 0 - 10V input channel 6 | mV |
| 0-10V_IN_7 | 30007 | 0 - 10V input channel 7 | mV |
| 0-10V_IN_8 | 30008 | 0 - 10V input channel 8 | mV |
| 1K_R_IN_1 | 30009 | 1K thernistor resistance channel 1 | ohm |
| 1K_R_IN_2 | 30010 | 1K thernistor resistance channel 2 | ohm |
| 1K_R_IN_3 | 30011 | 1K thernistor resistance channel 3 | ohm |
| 1K_R_IN_4 | 30012 | 1K thernistor resistance channel 4 | ohm |
| 1K_R_IN_5 | 30013 | 1K thernistor resistance channel 5 | ohm |
| 1K_R_IN_6 | 30014 | 1K thernistor resistance channel 6 | ohm |
| 1K_R_IN_7 | 30015 | 1K thernistor resistance channel 7 | ohm |
| 1K_R_IN_8 | 30016 | 1K thernistor resistance channel 8 | ohm |
| 10K_R_IN_1 | 30017 | 10K thernistor resistance channel 1 | ohm |
| 10K_R_IN_2 | 30018 | 10K thernistor resistance channel 2 | ohm |
| 10K_R_IN_3 | 30019 | 10K thernistor resistance channel 3 | ohm |
| 10K_R_IN_4 | 30020 | 10K thernistor resistance channel 4 | ohm |
| 10K_R_IN_5 | 30021 | 10K thernistor resistance channel 5 | ohm |
| 10K_R_IN_6 | 30022 | 10K thernistor resistance channel 6 | ohm |
| 10K_R_IN_7 | 30023 | 10K thernistor resistance channel 7 | ohm |
| 10K_R_IN_8 | 30024 | 10K thernistor resistance channel 8 | ohm |
| DRY_CONTACT_EDGE_COUNTER_CH1 | 30025 | Edge counter ch1 | - |
| DRY_CONTACT_EDGE_COUNTER_CH2 | 30026 | Edge counter ch2 | - |
| DRY_CONTACT_EDGE_COUNTER_CH3 | 30027 | Edge counter ch3 | - |
| DRY_CONTACT_EDGE_COUNTER_CH4 | 30028 | Edge counter ch4 | - |
| DRY_CONTACT_EDGE_COUNTER_CH5 | 30029 | Edge counter ch5 | - |
| DRY_CONTACT_EDGE_COUNTER_CH6 | 30030 | Edge counter ch6 | - |
| DRY_CONTACT_EDGE_COUNTER_CH7 | 30031 | Edge counter ch7 | - |
| DRY_CONTACT_EDGE_COUNTER_CH8 | 30032 | Edge counter ch8 | - |
| OWB_TEMP1| 30033 |  Temperature 1 | 0.01C|
| OWB_TEMP2| 30034 |  Temperature 2 | 0.01C|
| OWB_TEMP3| 30035 |  Temperature 3 | 0.01C|
| OWB_TEMP4| 30036 |  Temperature 4 | 0.01C|
| OWB_TEMP5| 30037 |  Temperature 5 | 0.01C|
| OWB_TEMP6| 30038 |  Temperature 6 | 0.01C|
| OWB_TEMP7| 30039 |  Temperature 7 | 0.01C|
| OWB_TEMP8| 30040 |  Temperature 8 | 0.01C|
| OWB_TEMP9| 30041 |  Temperature 9 | 0.01C|
| OWB_TEMP10| 30042 |  Temperature 10 | 0.01C|
| OWB_TEMP11| 30043 |  Temperature 11 | 0.01C|
| OWB_TEMP12| 30044 |  Temperature 12 | 0.01C|
| OWB_TEMP13| 30045 |  Temperature 13 | 0.01C|
| OWB_TEMP14| 30046 |  Temperature 14 | 0.01C|
| OWB_TEMP15| 30047 |  Temperature 15 | 0.01C|
| OWB_TEMP16| 30048 |  Temperature 16 | 0.01C|
| OWB_ID_A_TEMP1| 30049 |  ID A Temperature 1 | 64 bit ID |
| OWB_ID_B_TEMP1| 30050 |  ID B Temperature 1 | |
| OWB_ID_C_TEMP1| 30051 |  ID C Temperature 1 | |
| OWB_ID_D_TEMP1| 30052 |  ID D Temperature 1 | |
| OWB_ID_A_TEMP2| 30053 |  ID A Temperature 2 | 64 bit ID |
| OWB_ID_B_TEMP2| 30054 |  ID B Temperature 2 |  |
| OWB_ID_C_TEMP2| 30055 |  ID C Temperature 2 |  |
| OWB_ID_D_TEMP2| 30056 |  ID D Temperature 2 |  |
| OWB_ID_A_TEMP3| 30057 |  ID A Temperature 3 | 64 bit ID|
| OWB_ID_B_TEMP3| 30058 |  ID B Temperature 3 |  |
| OWB_ID_C_TEMP3| 30059 |  ID C Temperature 3 |  |
| OWB_ID_D_TEMP3| 30060 |  ID D Temperature 3 |  |
| OWB_ID_A_TEMP4| 30061 |  ID A Temperature 4 | 64 bit ID|
| OWB_ID_B_TEMP4| 30062 |  ID B Temperature 4 |  |
| OWB_ID_C_TEMP4| 30063 |  ID C Temperature 4 |  |
| OWB_ID_D_TEMP4| 30064 |  ID D Temperature 4 |  |
| OWB_ID_A_TEMP5| 30065 |  ID A Temperature 5 | 64 bit ID|
| OWB_ID_B_TEMP5| 30066 |  ID B Temperature 5 |  |
| OWB_ID_C_TEMP5| 30067 |  ID C Temperature 5 |  |
| OWB_ID_D_TEMP5| 30068 |  ID D Temperature 5 |  |
| OWB_ID_A_TEMP6| 30069 |  ID A Temperature 6 | 64 bit ID|
| OWB_ID_B_TEMP6| 30070 |  ID B Temperature 6 |  |
| OWB_ID_C_TEMP6| 30071 |  ID C Temperature 6 |  |
| OWB_ID_D_TEMP6| 30072 |  ID D Temperature 6 |  |
| OWB_ID_A_TEMP7| 30073 |  ID A Temperature 7 | 64 bit ID|
| OWB_ID_B_TEMP7| 30074 |  ID B Temperature 7 |  |
| OWB_ID_C_TEMP7| 30075 |  ID C Temperature 7 |  |
| OWB_ID_D_TEMP7| 30076 |  ID D Temperature 7 |  |
| OWB_ID_A_TEMP8| 30077 |  ID A Temperature 8 | 64 bit ID|
| OWB_ID_B_TEMP8| 30078 |  ID B Temperature 8 |  |
| OWB_ID_C_TEMP8| 30079 |  ID C Temperature 8 |  |
| OWB_ID_D_TEMP8| 30080 |  ID D Temperature 8 |  |
| OWB_ID_A_TEMP9| 30081 |  ID A Temperature 9 | 64 bit ID|
| OWB_ID_B_TEMP9| 30082 |  ID B Temperature 9 |  |
| OWB_ID_C_TEMP9| 30083 |  ID C Temperature 9 |  |
| OWB_ID_D_TEMP9| 30084 |  ID D Temperature 9 |  |
| OWB_ID_A_TEMP10| 30085 |  ID A Temperature 10 | 64 bit ID|
| OWB_ID_B_TEMP10| 30086 |  ID B Temperature 10 |  |
| OWB_ID_C_TEMP10| 30087 |  ID C Temperature 10 |  |
| OWB_ID_D_TEMP10| 30088 |  ID D Temperature 10 |  |
| OWB_ID_A_TEMP11| 30089 |  ID A Temperature 11 | 64 bit ID|
| OWB_ID_B_TEMP11| 30090 |  ID B Temperature 11 |  |
| OWB_ID_C_TEMP11| 30091 |  ID C Temperature 11 |  |
| OWB_ID_D_TEMP11| 30092 |  ID D Temperature 11 |  |
| OWB_ID_A_TEMP12| 30093 |  ID A Temperature 12 | 64 bit ID|
| OWB_ID_B_TEMP12| 30094 |  ID B Temperature 12 |  |
| OWB_ID_C_TEMP12| 30095 |  ID C Temperature 12 |  |
| OWB_ID_D_TEMP12| 30096 |  ID D Temperature 12 |  |
| OWB_ID_A_TEMP13| 30097 |  ID A Temperature 13 | 64 bit ID|
| OWB_ID_B_TEMP13| 30098 |  ID B Temperature 13 |  |
| OWB_ID_C_TEMP13| 30099 |  ID C Temperature 13 |  |
| OWB_ID_D_TEMP13| 30100 |  ID D Temperature 13 |  |
| OWB_ID_A_TEMP14| 30101 |  ID A Temperature 14 | 64 bit ID|
| OWB_ID_B_TEMP14| 30102 |  ID B Temperature 14 |  |
| OWB_ID_C_TEMP14| 30103 |  ID C Temperature 14 |  |
| OWB_ID_D_TEMP14| 30104 |  ID D Temperature 14 |  |
| OWB_ID_A_TEMP15| 30105 |  ID A Temperature 15 | 64 bit ID|
| OWB_ID_B_TEMP15| 30106 |  ID B Temperature 15 |  |
| OWB_ID_C_TEMP15| 30107 |  ID C Temperature 15 |  |
| OWB_ID_D_TEMP15| 30108 |  ID D Temperature 15 |  |
| OWB_ID_A_TEMP16| 30109 |  ID A Temperature 16 | 64 bit ID|
| OWB_ID_B_TEMP16| 30110 |  ID B Temperature 16 |  |
| OWB_ID_C_TEMP16| 30111 |  ID C Temperature 16 |  |
| OWB_ID_D_TEMP16| 30112 |  ID D Temperature 16 |  |

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
