 # CODESYS Driver

This is the Codesys driver for the [Building Automation Card](https://sequentmicrosystems.com/product/building-automation-stackable-card-for-raspberry-pi/)

We include the source code library in the package so everyone can modify. Note that it is an open source library with absolutely no warranty.
Checkout our example project for guidance.

For using multiple card in the same time you need to set the card stack lever from jumpers and modify the "I2C address" parameter of the  device, below you find the correspondence between stack level and hardware address:

| Stack Level | I2C address |
| --- | --- |
| 0 | 16#48 |
| 1 | 16#49 |
| 2 | 16#4A |
| 3 | 16#4B |
| 4 | 16#4C |
| 5 | 16#4D |
| 6 | 16#4E |
| 7 | 16#4F |

 # Important Notice
We are thankful to [nikke344](https://github.com/nikke344) who helped to develop the library.
