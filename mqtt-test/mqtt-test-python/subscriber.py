import paho.mqtt.client as mqtt

BROKER = "localhost"
PORT = 1883
TOPIC = "test/topic"

# Callback when message is received
def on_message(client, userdata, msg):
    print(f"Received on {msg.topic}: {msg.payload.decode()}")

def main():
    client = mqtt.Client()
    client.on_message = on_message

    client.connect(BROKER, PORT, 60)
    client.subscribe(TOPIC)

    print(f"Subscribed to {TOPIC}, waiting for messages...")
    client.loop_forever()

if __name__ == "__main__":
    main()
