# MQTT Test Applications (C++)

## Setup QuickStart
1. `sudo apt-get update`
2. `sudo apt-get install -y build-essential cmake libmosquitto-dev`

## Build
1. `cmake -S . -B build`
2. `cmake --build build -j`

## Quick Start
1. Run MQTT Broker `mosquitto -c /etc/mosquitto/mosquitto.conf -v`
2. Run publisher `./build/publisher`
3. Run subscriber `./build/subscriber`
