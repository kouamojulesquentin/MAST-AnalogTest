----------- Rebuilding OpenOCD for MAST -----------------------

OpenOCD is a third-party software used by MAST for access interface and adapter support.

As the number of available adapters on the market is high, OpenOCD proposes to compile it for a set of adapters the user wants,
reducing dependencies with other third-party libraries.
OpenOCD establishes the list of drivers that must be generated during the ./configure process,
where already installed third-party libraries for bus-handling (USB, RS232, or exotic adapter bus) are detected.
A summary of all supported adapter dongles is given at the end of this process.

It is possible to cover a major part of the supported adapters at the moment libusb0 or libusb1 is installed.
Some users may wish to work with FT2232-based USB dongles.
In such a case, we recommend to use of the libFTDI library.


MAST provides an automated script compiling OpenOCD source code ready to be exported to MAST. There is no need to move any output file. Just recompile OpenOCD first, then compile MAST.

The following process must be done, from this directory (openocd/openocd-builder):
  - create a 'build' directory            (mkdir build)
  - move to the 'build' directory         (cd build)
  - invoke cmake to the parent directory  (cmake ..)
  - enter 'make' to generate the corresponding library and headers according to available local interfaces
  - then type 'make install' to copy these generated files into the internal openocd hierarchy used by MAST.

----------- Troubleshooting OpenOCD building for MAST  -----------------------

Q: Is my Debug Adapter supported by OpenOCD/MAST?

A: You can find a list of all adapters supported by OpenOCD on their website: http://openocd.org/doc/html/Debug-Adapter-Hardware.htm . MAST currently provides OpenOCD version 0.9.0.

--

Q: How can I establish the list of the drivers generated?

A: If you have already built OpenOCD for MAST, please check the build/OpenOCD-prefix/src/OpenOCD-build/config.h file.
   Constants with the '1' value are the enabled drivers for your installation.

   If there is no build yet ready or if you are not familiar with reading a .h file,
   untar openocd_mast.tar.gz (tar -xzvf openocd_mast.tar.gz), and then execute './configure' in the newly created 'openocd' directory.
   A list of all supported adapters for your machine will be given at the end of the script execution.

--

Q: The connection with my adapter/JTAG TAP/SPI slave does not work. How to check if the problem comes from the adapter?

A: You can run directly openocd by executing the build/openocd_build/openocd/build/bin/openocd binary with the following options
-f <adapter_configuration_file> -f <sut_description_file_for_openocd> -c "init; <list of commands>"

The SUT description file for OpenOCD can be design according to the guidelines given on http://openocd.org/doc/html/Config-File-Guidelines.html .

In boundary scan mode, commands can be found on http://openocd.org/doc/html/Boundary-Scan-Commands.html . You can add commands as many as you want, at the moment they are separated by a semicolon (';').

--

Q: Why there is an external CMake script doing exactly the same job than the standard OpenOCD installation commands?
   Why I should not just install OpenOCD on my computer?

A: OpenOCD is originally provided as an executable and not as a shared library.
   Furthermore, MAST directly handles JTAG requests via the JTAG driver provided by OpenOCD and does not pass by another socket server.
   This CMake script endorses all linking jobs according to the third-party libraries installed on the local machine to generate the
   shared library MAST uses.
   It also copies the header and library files in the local MAST hierarchy.
