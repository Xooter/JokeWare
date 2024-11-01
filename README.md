# JokeWare

## Overview

This project consists of two programs: an FTP server and a client, both developed in C++ using CMake. The main purpose of this application is to have fun by executing various prank commands on a friend's computer. Please ensure that you have permission before using this tool on anyone's machine.

## Features

The FTP server operates on port **6665** and includes the following functionalities:

- **Change Mouse Sensitivity:** Modify the sensitivity settings of the mouse.
- **Erratic Mouse Movement:** Make the mouse move erratically on the screen.
- **Change Wallpaper:** Change the desktop wallpaper to a predefined image.
- **Execute OS Commands:** Send commands directly to the operating system.
- **Open Dialog Box:** Display dialog boxes for various purposes.
- **Adjust Volume:** Change the system volume.
- **Minimize:** Minimize the current window.
- **Screensaver:** Set on the system screensaver.
- **Resolution:** Change the system resolution.
- **Keyboard:** Change system keyboard language.
- **Hide Mouse:** Change the mouse visibility.

## Setup Instructions

### Requirements

1. Ensure you have a compatible C++ compiler installed (e.g., g++, Visual Studio).
2. Install CMake for building the projects.
3. It is highly recommended that your friend has either Radmin or Hamachi installed. This allows remote access via IP without the need to open ports manually.

### Installation

1. Clone the repository to your local machine.

```bash
git clone https://github.com/Xooter/JokeWare
cd JokeWare
```

2. Build the project using CMake.

> Run in the terminal

```bash
mkdir build
cd build
cmake ..
make
```

> Or run the build.sh script

```bash
./setup.sh
```

3. Run the FTP server on your friend's computer.

```bash
./Batman
```

4. Use the client program to send commands to the server when connected.

```bash
./Joker [command] [params]
```

### Connecting Remotely

To enable easier connection between the client and server, follow these steps:

1. Have your friend run Radmin or Hamachi to establish a virtual network.
2. Locate the IP address assigned to your friend's machine through the software.
3. Connect to the FTP server using that IP address.

## Usage

After the server is running and your client is connected, you can execute any of the available prank commands by entering them into the client. Here are some examples of commands you can use:

- `change_mouse_sensitivity -> Example: mouse <1 - 20>`
- `move_mouse_erratically -> Example: mousemove 5 (segs)`
- `change_wallpaper -> Example: wallpaper <path>`
- `execute_os_command -> Example: os <command>`
- `open_dialog_box -> Example: dialog <message>`
- `adjust_volume -> Example: volume <1 - 100>`
- `minimize -> Example: minimize`
- `screensaver -> Example: screensaver`
- `resolution -> Example: resolution <width>x<height>`
- `keyboard -> Example: keyboard [<code>]` -> [codes](https://learn.microsoft.com/es-es/windows-hardware/manufacture/desktop/windows-language-pack-default-values?view=windows-11)
- `hide_mouse -> Example: hide_mouse`

## Warning

Please remember to use this program responsibly and only with friends who would appreciate these kinds of jokes. Make sure to inform them afterward to avoid causing any distress or frustration.

## License

This project is licensed under the MIT License - see the LICENSE file for details.
