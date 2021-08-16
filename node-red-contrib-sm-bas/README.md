# node-red-contrib-sm-bas

This is the node-red node to control Sequent Microsystems [Building Automation Stackable Card for Raspberry Pi](https://sequentmicrosystems.com/products/raspberry-pi-building-automation).

## Manual Install

Clone or update the repository, follow the instrutions fron the [first page.](https://github.com/SequentMicrosystems/megabas-rpi)

In your node-red user directory, tipicaly ~/.node-red,

```bash
~$ cd ~/.node-red
```

Run the fallowing command:

```bash
~/.node-red$ npm install ~/megabas-rpi/node-red-contrib-sm-bas
```

In order to see the node in the palette and use-it you need to restart node-red. If you run node-red as a service:
 ```bash
 ~$ node-red-stop
 ~$ node-red-start
 ```

## Usage

After install and restart the node-red you will see on the node palete, under Sequent Microsystems category 7 new nodes:

### BAS triac

This node will turn on or off a triac. 
The card stack level and triac number can be set in the dialog screen or dynamicaly through ``` msg.stack``` and ``` msg.triac ```.
The output of the triac can be set dynamically as a boolean using ```msg.payload```.

### BAS 0 10V in

This node reads one 0-10V input channel (Make sure the corresponding jumper is in the "0-10V" position in order for this read to be correct).
The card stack level and channel number can be set in the node dialog box or dynamically through ```msg.stack``` and ```msg.channel```.
The read is triggered by the message input and output can be found in the output message payload as a number representing the voltage readings in volts.

### BAS 0 10V out

This node controls one 0-10V output channel.
The card stack level and channel number can be set in the node dialog box or dynamically through ```msg.stack``` and ```msg.channel```.
The value in volts is set dynamically as a number between 0..10 thru ```msg.payload```.

### BAS R 1K in

This node reads the 1K ohms thermistor input channel(Make sure the corresponding jumper is in the "1K" position in order for this read to be correct).
The card stack level and channel number can be set in the node dialog box or dynamically through ```msg.stack``` and ```msg.channel```.
The read is triggered by the message input and output can be found in the output message payload as a number representing the resistance readings in kiloohms.

### BAS R 10K in

This node reads the 10K ohms thermistor input channel(Make sure the corresponding jumper is in the "10K" position in order for this read to be correct).
The card stack level and channel number can be set in the node dialog box or dynamically through ```msg.stack``` and ```msg.channel```.
The read is triggered by the message input and output can be found in the output message payload as a number representing the resistance readings in kiloohms.

### BAS DC cnt

This node reads the dry contact input cunter for one particular channel and set the counting edges for that channel.
The card stack level and dry contact inplut counter channel number can be set in the node dialog box or dynamically through ```msg.stack``` and ```msg.channel```.
The rising edge counting and/or falling edge counting can be enabled/diabled with coresponding check box in the node dialog. The edge settings are sent to the card every time you deply nthis node or the flow start or you select different channel for read through ```msg.channel```.
The read is triggered by the message input and output can be found in the output message payload.

### BAS DC in

This node reads the dry contact input state for one particular channel(Make sure the corresponding jumper is in the "1K" position in order for this read to be correct).
The card stack level and dry contact inplut channel number can be set in the node dialog box or dynamically through ```msg.stack``` and ```msg.channel```.
The read is triggered by the message input and output can be found in the output message payload as boolean.

## Important note

This node is using the I2C-bus package from @fivdi, you can visit his work on github [here](https://github.com/fivdi/i2c-bus). 
The inspiration for this node came from @nielsnl68 work with [node-red-contrib-i2c](https://github.com/nielsnl68/node-red-contrib-i2c).Thank both for the great job.
