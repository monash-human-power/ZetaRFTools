# ZetaRF Tools

[![All Contributors](https://img.shields.io/badge/all_contributors-2-orange.svg?style=flat-square)](#contributors)

A collection of tools for experimenting with the ZetaRF radio module.

## Installation

Clone the repository and initialise submodules with `git clone <url> --recurse-submodules`. If you've already cloned, run `git submodule update --init` to set up the submodules.

You will need [CMake](https://cmake.org/) installed to build. Keep in mind that this project is intended to be built on the Pi, it won't compile on your local machine.

Initialise the CMake project with `cmake .` in the root directory, and finally build by running `make`.

The binaries are built to the new `build` subdirectory, where they may be run with `./build/<tool name>`.

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
