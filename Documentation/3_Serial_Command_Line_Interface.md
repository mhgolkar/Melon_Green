# Melon Green
## Serial Command Line Interface
Serial communication with your Melon Green, lets you to configure it. There is also other functions for diagnostics or reading sensors.

### Connection
#### Standalone version
1.  Prepare an **USB to Serial adapter/converter** and appropriated cable, like [FTDI](http://www.ftdichip.com/Products/Cables/USBTTLSerial.htm)s.  
*Connect the adapter to host ( PC, Smart-phone etc.), then check for availability.*
*Install or Update Drivers if needed.*
2.  **Find COM / Serial Port Name**  
   Under **Mac**, in the *Terminal* window, type in `ls /dev/cu.*`. The port name which we need later is `/dev/cu.usbserial-###` where the #s are going to be unique for each cable.  
   For **Linux/Unix**, type `ls /dev/ttyUSB*`. You'll receive something like `ttyUSB0`.  
   If you are using **Windows**, go to the Device Manager and look for an entry under `Ports (COM & LPT)`. That's like `USB Serial Port (COM#)`. If you don't find the COM port, make sure that the cable is plugged in, and check if you installed the required driver like [VCP Driver(s) for FTDI devices](http://www.ftdichip.com/Drivers/VCP.htm).  
   For windows set the port's *baud rate* to 9600 (board's default).
3.  **Connect the M.G. board to the host via adapter**:  
   *Adapter's Rx to Board's Tx, and vice versa aTx to bRx.*  
   *Remember Common Ground (aGND to bGND).*  

#### Arduino version
1.  Connect the duino to host via USB cble  
2.  Find COM / Serial Port Name:  
   Take a look at Arduino IDE's Status-bar or `Menu-bar -> Tools -> port`.

### Communication
1.  **Setup a reliable serial monitor application**  
   Note: Don't use *Arduino IDE Serial Monitor*, or you'll find yourself in a mess. *It doesn't have some \[standard] features like page breaks*. More comfortable Alternatives are:  
  ▧ [PuTTY](http://www.putty.org/) \[Windows, Linux]  
  ▧ Screen \[Mac, Linux]  
2.  **Open and configure serial monitor**  
   Note: Melon Green's default baud rate is `9600`, but you can change it from `Melon_Green/modules/Presets.h` file.  
   **Mac/Linux**: Type in Terminal: `screen <serial_port_name> <baud_rate>`.  
   *Note! If you faced to some error like this: `bash: /dev/ttyUSB#: Permission denied`, be brave and type: `sudo chmod 666 /dev/ttyUSB#` then `sudo screen /dev/ttyUSB# 9600
   `*  
   **Windows**: Run `PuTTY`. In *Session* select *Serial* Radio, then change *Serial line* to *COM#* and Speed to baud rate (9600). From Categories, select *Terminal*, then from the *Line discipline options* fieldset, select both *'Force on'* radios. From Categories, select *Connection/Serial*, change *Flow control* to *none*. click on *Open* button.    
3.  Restart board or type ``0`` (Zero). You'll see the CLI Menu.

![Serial Communication in Action](3_Serial_Command_Line_Interface/Serial_Communication_PuTTY.gif)  

It's Alive!
