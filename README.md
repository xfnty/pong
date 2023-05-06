
## Developing

### Installing dependencies
On Linux
```
sudo apt install -y build-essential git cmake libasound2-dev mesa-common-dev libx11-dev libxrandr-dev libxi-dev xorg-dev libgl1-mesa-dev libglu1-mesa-dev
```
On Windows
- [Install Python3](https://www.python.org/downloads/)
- [Install CMake](https://cmake.org/download/)
- [Check out Raylib installation guide](https://github.com/raysan5/raylib/wiki/Working-on-Windows) (everything should work anyway)

### Building
Run `toolchain.py` from the project root:
```
python toolchain.py -cbr
```
Run `./toolchain.py -h` to see all available options.
