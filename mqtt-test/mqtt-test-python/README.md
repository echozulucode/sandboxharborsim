# MQTT Test Applications

## Setup Quick Start
1. `cd mqtt-test`
2. `uv init .` .. this is a rust based alternative to pip
3. `uv add paho-mqtt`


## Quick Start

1. Run MQTT Broker `mosquitto -c /etc/mosquitto/mosquitto.conf -v`
2. Run publisher `uv run publisher.py`
3. Run subscriber `uv run subscriber.py`

### Debug `/etc/mosquitto/mosquitto.conf`
```
listener 1883 0.0.0.0
allow_anonymous true
persistence true
persistence_location /var/lib/mosquitto/
```