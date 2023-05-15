From an Admin Windows Prompt, list the UBD devices:

> usbipd wsl list

Identify the "bus-id" of your FTDI device. Ex : here it is 6-2

BUSID  VID:PID    DEVICE                                                        STATE
3-4    0bda:8153  Realtek USB GbE Family Controller #2                          Not attached
5-5    413c:b06e  Périphérique d'entrée USB                                     Not attached
6-2    0403:6011  USB Serial Converter A, USB Serial Converter B, USB Seria...  Not attached
6-4    8087:0025  Intel(R) Wireless Bluetooth(R)                                Not attached
6-12   0c45:671d  Integrated Webcam                                             Not attached
7-2    046d:c52b  Logitech USB Input Device, Périphérique d'entrée USB          Not attached
7-4    0bda:402e  Realtek USB Audio                                             Not attached
7-5    413c:b06f  Périphérique d'entrée USB                                     Not attached

Check if you can see your WSL distro, and if it set as default: 

> wsl -l


----------------------------------------------------------------------------
OPTION 1: you CAN see your WSL distro from Windows Admin Prompt: 
----------------------------------------------------------------------------
From the Admin Windows Prompt, attach the device to the default WSL

> usbipd wsl attach --busid <busid>

From the WSL, you can now see and use the device
When finished, detach it: 

usbipd wsl detach --busid <busid>

----------------------------------------------------------------------------
OPTION 2: you CANNOT see your WSL distro from Windows Admin Prompt: 
----------------------------------------------------------------------------

The command usbipd wsl attach --busid <busid> won't work since my regular user don't have admin rights, and my Admin account doesn't see the WSL distro. The solution is to split this command in two:

In CommandPrompt or PowerShell with Admin rights run the following command: 
usbipd bind --busid <busid> --force 
If you don't use --force the next step will fail with a "Device busy" error.


Then execute OPTION 1

NB: remember to unbind it after it is detached:

usbipd unbind --busid <busid> 

-----------------------------
BACKUP: 
Make sure that you have completely restarted WSL (Run wsl --shutdown in a CommandPrompt or PowerShell). 

Then in WSL run sudo usbip attach -r $HOSTNAME.local --busid=<bus-id>. (Without sudo you will get an "Import Error".
