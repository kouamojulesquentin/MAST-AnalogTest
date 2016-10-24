These prototypes are based on the usage of a usb dongle incorporating a FTDI FT232H chip

The chip provides notably two Digital bused: AD and BD (unused yet). Here is the pinout for the two example breakout boards: 

 - Dangerous Prototypes: 
  Pinout direcly printed on PCB
          JTAG           SPI
  AD0 --> TCLK      -->  SCK
  AD1 --> TDI       -->  MOSI
  AD2 --> TDO       -->  MISO
  AD3 --> TMS       -->  SS_N
  AD4 --> TRST
 
 - FT2232H Mini ModulE: 
                    JTAG           SPI
  AD0 - CN2-07 --> TCLK      -->  SCK
  AD1 - CN2-10 --> TDI       -->  MOSI
  AD2 - CN2-09 --> TDO       -->  MISO
  AD3 - CN2-12 --> TMS       -->  SS_N
  AD4 - CN2-14 --> TRST
  AD5 - CN2-13
  AD6 - CN2-16
  AD7 - CN2-15
  
NB: SS_N can be configured with libftdispi on GPIO Adbus 4 to 7

  
