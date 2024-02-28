# iot-device firmware

Main tasks of IoT device are measuring temperature, pressure and humidity, controlling relays and LEDs. 

Device is based on:
* [AT32F437 Series](https://www.arterychip.com/en/product/AT32F437.jsp) MCU by 
  [ArteryTek][https://www.arterychip.com/], specifically [AT_START_F437_V1.1](https://www.arterychip.com/download/AT-START/AT_START_F437_V1.1.zip)
  board
* Digital pressure/temperature/humidity sensor [BME280](https://www.bosch-sensortec.com/products/environmental-sensors/humidity-sensors-bme280/)


## Programming via JTAG

As JTAG for Artery MCUs can be used either [AT-Link EZ](https://www.arterychip.com/download/TOOL/UM0004_AT-Link_User_Manual_EN_V2.1.1.pdf) 
or [ST-Link/V2](https://www.st.com/en/development-tools/st-link-v2.html), but with custom  [OpenOCD](https://github.com/ArteryTek/openocd) 
provided by ArteryTek. Corresponding configurations for OpenOCD are provided in [openocd](openocd) directory.


## In-system programming

In-system programming (ISP) tools:
* [programmer](https://www.arterytek.com/download/TOOL/Artery_ISP_Programmer_V2.0.11.zip);
* [console](https://www.arterychip.com/download/TOOL/Artery_ISP_Console_Linux-x86_64_V3.0.08.zip).

Programming procedure:
1. Download ISP console
1. Configure ISP console: 
   ```shell
   ./AT32_ISP_Console.sh
   ```
1. Connect PC and the board via USB
1. Put the MCU in device firmware update (DFU) mode: switch BOOT0 jumper to VDD and reset the board
1. Load flash memory with command:
   ```shell
   ./AT32_ISP_Console -dfu -e --all -d --a 0x08000000 --v --fn "/absolute/path/to/firmware.bin"
   ```

Successful programming log:

```shell
------------------------------------------------------------
Artery ISP Programmer V3.0.08
Support USART and DFU
------------------------------------------------------------

DFU Connect......
Device:  AT32F437ZMT7    Flash:  4032 KB
PID:  7008454F      BID:  4A03    Version:  3.2
Device connected Successfully

Erase
Running...100%
Erase successfully!
Download
build/iot-device.bin     4212B
Running...100%
Verify
build/iot-device.bin     4212B
Running...100%
Download successfully!
```


## Connecting device via USB

Device is configured as CDC device (Virtual COM port).

To communicate with device any COM/Terminal application can be used, for example:
* [minicom](https://help.ubuntu.com/community/Minicom)
* [CoolTerm](https://freeware.the-meiers.org/)

Example of connection using `minicom`:
```shell
minicom -wc on --baudrate 115200 --device /dev/ttyACM1
```
