# ZetaRF Tools

[![All Contributors](https://img.shields.io/badge/all_contributors-2-orange.svg?style=flat-square)](#contributors)

A collection of tools for experimenting with the ZetaRF radio module.

## Installation

1. Clone the repository and initialise submodules
   ```bash
    git clone <url> --recurse-submodules
   ```
   If the repository is already cloned, use
   ```bash
   git submodule update --init
   ```

2. Install [CMake](https://cmake.org/). Keep in mind that this project is intended to be built on the Pi, it won't compile on your local machine.

3. Initialise the CMake project and build with
    ```bash
    cmake .
    make
    ```

4. The binaries are located in the new `build` sub directory. They may be run with
    ```bash
    ./build/<tool name>
    ```

## Tools

The following is a list of tools present in this repository.

Name          | Description
--------------|-------------
AverageRSSI   | Listens for any and all packets, and measures the Received signal strength indication (RSSI) of each. You can set the duration the test runs for, and the expected number of packets for calculating drop percentages.
BandwidthTest | Tries to send a specified number of packets of a specified size as quickly as possible, and measures the speed that it does so.
SimpleRx      | Prints all received messages as text and hex, as well as their RSSI.
Beacon        | Sends a packet repeatedly at a given interval

## Contributors ✨

Thanks goes to these wonderful people ([emoji key](https://allcontributors.org/docs/en/emoji-key)):

<!-- ALL-CONTRIBUTORS-LIST:START - Do not remove or modify this section -->
<!-- ALL-CONTRIBUTORS-LIST:END -->

This project follows the [all-contributors](https://github.com/all-contributors/all-contributors) specification. Contributions of any kind welcome!
