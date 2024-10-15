
[![megabas-rpi](res/sequent.jpg)](https://sequentmicrosystems.com)

# megabas

This is the python library to control the [ Building Automation Card for Raspberry Pi.](https://sequentmicrosystems.com/products/raspberry-pi-building-automation) 

## Install

```bash
sudo pip install SMmegabas
```

## Usage

Now you can import the megaio library and use its functions. To test, read triacs status from the MegaIO IND board with stack level 0:

```bash
~$ python
Python 2.7.9 (default, Sep 17 2016, 20:26:04)
[GCC 4.9.2] on linux2
Type "help", "copyright", "credits" or "license" for more information.
>>> import megabas
>>> megabas.getTriacs(0)
0
>>>
```

## Functions

### def getVer(stack)
Return firmware version

stack - stack level of the megabas  card (selectable from address jumpers [0..8])


### setUOut(stack, ch, value)
Set the selected output 0-10V channel value in volts

stack - stack level of the megabas  card (selectable from address jumpers [0..8])

ch - selected channel number [1..4]

value - voltage output value in V [0..10]


### getUOut(stack, ch)
Get the selected output 0-10V channel value in volts  

stack - stack level of the megabas  card (selectable from address jumpers [0..8])

ch - selected channel number [1..4]

return - value in V [0..10]


### getUIn(stack, ch)
Return the selected input 0-10V channel value in volts

stack - stack level of the megabas  card (selectable from address jumpers [0..8])

ch - selected channel number [1..8]

return - value in V [0..10]


### getRIn1K(stack, ch)
Return the selected resistance (1K) measurements in kilo Ohms. This measurement is valid only if the jumper is place in "1K" position.
On this type of input is recomended to measure 1kOhm thermistor. 

stack - stack level of the megabas  card (selectable from address jumpers [0..8])

ch - selected channel number [1..8]

return - value in kOhm [0..30] were 30 means invalid measurements


### getRIn10K(stack, ch)
Return the selected resistance (10K) measurements in kilo Ohms. This measurement is valid only if the jumper is place in "10K" position.
On this type of input is recomended to measure 10K thermistor. 

stack - stack level of the megabas  card (selectable from address jumpers [0..8])

ch - selected channel number [1..8]

return - value in kOhm [0..30]


### setTriac(stack, ch, val)
Set one triac state.

stack - stack level of the megabas  card (selectable from address jumpers [0..8])

ch - triac number (id) [1..4]

val - triac state 1: turn ON, 0: turn OFF[0..1]


### setTriacs(stack, value)
Set all triacs state.

stack - stack level of the megabas  card (selectable from address jumpers [0..8])

value - 4 bit value of all triacs (ex: 15: turn on all triacs, 0: turn off all triacs, 1:turn on triac #1 and off the rest)


### getTriac(stack, ch)
Return the state of one triac.

stack - stack level of the megabas  card (selectable from address jumpers [0..8])

ch - triac number (id) [1..4]

return - (0/1)


### getTriacs(stack)
Return the state of all triacs.

stack - stack level of the megabas  card (selectable from address jumpers [0..8])

return - [0..15]


### togleTriac(stack, ch, delay, count)
Togle one triac state.

stack - stack level of the megabas  card (selectable from address jumpers [0..8])

ch - triac number (id) [1..4]

delay - delay between togles 

count - number of togles


### getContact(stack)
Get the state of the dry contact inputs

stack - stack level of the megabas  card (selectable from address jumpers [0..8])

return - value of the inputs [0..255]


### getContactCh(stack, ch)
Get the state of the dry contact input channel

stack - stack level of the megabas  card (selectable from address jumpers [0..8])

ch - selected channel number [1..4]

return - value of the inputs [0/1]


### getContactCounter(stack, ch)
Return the counter value for corresponding dry contact input.

stack - stack level of the megabas  card (selectable from address jumpers [0..8])

ch - selected channel number [1..4]

return - counter value (32bits)


### getContactCountEdge(stack, ch)
Return dry contact edge settings for coresponding channel

stack - stack level of the megabas  card (selectable from address jumpers [0..8])

ch - selected channel number [1..4]

return: 0 - edge count disable; 1 - rising edge count enabled; 2 - falling edge count enabled; 3 - both edges count enabled


### setContactCountEdge(stack, ch, edge)
Set dry contact edge count mode

stack - stack level of the megabas  card (selectable from address jumpers [0..8])

ch - selected channel number [1..4]

edge: 0 - edge count disable; 1 - rising edge count enabled; 2 - falling edge count enabled; 3 - both edges count enabled


### getInVolt(stack)
Get the power supply voltage

stack - stack level of the megabas  card (selectable from address jumpers [0..8])

return - the power supply in Volts


### getRaspVolt(stack)
Get the raspberry power supply voltage (5V)

stack - stack level of the megabas  card (selectable from address jumpers [0..8])

return - the Raspberry pi power supply in Volts


### getCpuTemp(stack)
Get the cpu temperature 

stack - stack level of the megabas  card (selectable from address jumpers [0..8])

return - temperature in deg Celsius


### wdtGetPeriod(stack)
Return the current period of the watchdog timer in seconds

stack - stack level of the megabas  card (selectable from address jumpers [0..8])


### wdtSetPeriod(stack, val)
Set the period of the watchdog in seconds, val = 65000 disable the watchdog

stack - stack level of the megabas  card (selectable from address jumpers [0..8])

val - [10..65000]


### wdtReload(stack)
Reload the watchdog timer with the current period.
The next reload command must occur in no more the "period" time in order to prevent watchdog to re-power the Raspberry.
This command also enables the watchdog if is disabled (power-up disabled).

stack - stack level of the megabas  card (selectable from address jumpers [0..8])


### wdtSetDefaultPeriod(stack, val)
This function updates the period that will be loaded after Raspberry power is turned off and back on. You must set this period long enough to let Raspberry boot-up and your "watchdog maintaining" script to start.

stack - stack level of the megabas  card (selectable from address jumpers [0..8])

value - [10...64999] seconds


### wdtGetDefaultPeriod(stack)
Return the default period 

stack - stack level of the megabas  card (selectable from address jumpers [0..8])

value - [10...64999] seconds


### wdtSetOffInterval(stack, val)
Set the time interval in seconds for keeping Raspberry power off in case of watchdog timer expire. 

stack - stack level of the megabas  card (selectable from address jumpers [0..8])

val - [10...4147200] seconds 


### wdtGetOffInterval(stack)
Return the Off time interval in seconds

stack - stack level of the megabas  card (selectable from address jumpers [0..8])

return - [10...4147200] seconds 


### wdtGetResetCount(stack)
Return the numbers of Raspberry re-powers performed by the watchdog 

stack - stack level of the megabas  card (selectable from address jumpers [0..8])

return - [0..65535]

## RTC Functions

### rtcGet(stack)
Return the RTC date and time as a list

stack - stack level of the megabas  card (selectable from address jumpers [0..7])

return (year, month, day, hour, minute, seconds)

### rtcSet(stack, y, mo, d, h, m, s)
Set the RTC date and time

stack - stack level of the megabas  card (selectable from address jumpers [0..7])

y - year between 2000..2255 or between 0..255

mo - month 1..12

d - day

h - hour

m - minutes

s - seconds

## Owire Bus Functions

### owbScan(stack)
Start scanning for connected sensors

 stack - stack level of the megabas  card (selectable from address jumpers [0..7])

### owbGetSensorNo(stack)
Get the numbers of 18B20 sensors connected on the bus

 stack - stack level of the megabas  card (selectable from address jumpers [0..7])

 return number of connected sensors

### owbGetTemp(stack, sensor)
Read the temperature aquired by one sensor

 stack - stack level of the megabas  card (selectable from address jumpers [0..7])
 
 sensor - sensor number [1..16]

 return temperature in degree Celsius
 
 ### owbGetRomCode(stack, sensor)
 Read the unic ROM code of one sensor
 
  stack - stack level of the megabas  card (selectable from address jumpers [0..7])
 
  sensor - sensor number [1..16]

  return ROM code as 8 bytes array
