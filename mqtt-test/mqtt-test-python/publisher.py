import paho.mqtt.client as mqtt
import time

BROKER = "localhost"   # or the IP/hostname of your Mosquitto broker
PORT = 1883
TOPIC = "test/topic"

def main():
    client = mqtt.Client()
    client.connect(BROKER, PORT, 60)

    count = 0
    while True:
        message = f"Hello MQTT {count}"
        client.publish(TOPIC, message)
        print(f"Published: {message}")
        count += 1
        time.sleep(2)

if __name__ == "__main__":
    main()
