# GUST_Comm

Communication between GUST and a 3rd-party software

## Getting started

Right it is working only on Windows using socket (UDP or TCP).
Support for Linux and UART will be added.

## Windows Build (Cmake and Visual Studio)

2. Clone [this repo](https://git.psu.edu/purl/psu-uav-research-laboratory/gust_comm)
3. Install and Open Cmake [CMake-gui](https://cmake.org/download/).
4. Set source to `<your_repo>/GUST_Comm`.
5. Set buildto `<your_repo>/GUST_Comm/build`.
6. Click `Configure`, `Generate` Use your verson of visual studio's compiler.
7. Open the solution in visual studio.
8. Right click on gust_comm in the solution explorer (right window). Set Properties -> Configuration Properties (left menu) -> Debugging -> Working Directory as the root directory of the gust_comm folder
9. Right click on gust_comm in the solution explorer. "Set as startup project" from the context menu (optional)

## Linux Build

1. Using CMake
```
mkdir build
cd build
cmake ..
make
./gust_comm
```

or 

2. Compile directly using `gcc`
```
gcc GUST_Comm.cpp datalink.cpp ether2.cpp serial.cpp -lstdc++ -o gust_comm
```

## Change IP Addresses

On this program side
1. Go to `config.h`
2. Change the ip address according to comments in the file.

On GUST side
1. Go to `test_GUST_Comm.inp`
2. Change the ip address according to comments in the file.

##  Integrating the gust_comm Library into Another Windows Software

The required files are:
datalink.cpp
datalink.h
ether.cpp (windows)
serial.h
serial.cpp

- serialPort_ref and datalink_ref must be defined.
- readDatalink and writeDatalink must be called on each timestep. They are nonblocking.
- writeDatalink must be modified to use the required messages and the appropriate structs must be populated following the example.
- The "data->header->messageID" switch case statement in readDatalink must contain the desired message type. It's struct will then be populated and available for use.