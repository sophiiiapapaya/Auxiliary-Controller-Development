# Auxiliary-Controller-Development

These directions assume you have performed the toolchain installation in chapter 9 of [the getting started guide](https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf) for your operating system.

# Preparing the SDK
1. After cloning/downloading this repository to your computer, download the pico SDK [here](https://github.com/raspberrypi/pico-sdk) and place it in a separate directory
2. Within the pico SDK, go to the external folder and copy the file pico_sdk_import to the Auxiliary_Controller folder in this repository
3. At the top of the file you copied, add the following line, where "path-to-sdk" is replaced by the full absolute path to wherever you are storing the pico SDK on your computer (keep the quotation marks, and use forward slashes):  
    set(PICO_SDK_PATH "path-to-sdk")

# Directions for Building Code
1. Run the following command from within the Auxiliary_Controller/build directory  
    cmake -G "NMake Makefiles" ..
2. Run the nmake command from within the same directory

# Directions for Flashing Code
1. To enter bootloader mode, either hold down the button on the Pico with the cable unplugged and release the button after plugging it in, or press the external reset button followed by the button on the Pico and then release the buttons in the same order (with the cable plugged in)
2. Copy the .uf2 file that was generated during the build process into the mass storage device that appears
3. The Pico will switch from a mass storage device to a serial port and begin running the code. The code can be restarted at any time by pressing the external reset button
