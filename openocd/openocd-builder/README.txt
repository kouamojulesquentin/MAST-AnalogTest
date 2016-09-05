----------- Rebuilding OpenOCD for MAST -----------------------

OpenOCD is a third-party software used by MAST for access interface and adapters support.

As the number of adapters on the market is important, OpenOCD can be enabled only for those for which a driver library has already been installed on the machine. A major part are already supported at the moment libusb0 or libusb1 is installed. However, some users may require, due to operating system restrictions or proprietary drivers, to recompile OpenOCD.

MAST provides an automated script compiling OpenOCD source code ready to be exported to MAST. There is no need to move any output file. Just recompile OpenOCD first, then compile MAST.

The following process must be done, from this directory:
- create a 'build' directory (mkdir build)
- move to the 'build' directory (cd build)
- invoke cmake to the parent directory (cmake ..)
- enter 'make' to generate the corresponding library and headers according to available local interfaces
- then type 'make install' to copy these generated files into the internal openocd hierarchy used by MAST.
