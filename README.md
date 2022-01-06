# Auxiliary-Controller-Development

# Preparing the SDK
1. After cloning/downloading this repository to your computer, run the following command in the base directory of the local copy
    git submodule update --init --recursive

# Directions for Building Code
1. Run the following command from within the Auxiliary_Controller/build directory
    cmake -G "NMake Makefiles" ..
2. Run the nmake command from within the same directory

# Directions for Flashing Code
1. To enter bootloader mode, either hold down the button on the Pico with the cable unplugged and release the button after plugging it in, or press the external reset button followed by the button on the Pico and then release the buttons in the same order (with the cable plugged in)
2. Copy the .uf2 file that was generated during the build process into the mass storage device that appears
3. The Pico will switch from a mass storage device to a serial port and begin running the code. The code can be restarted at any time by pressing the external reset button