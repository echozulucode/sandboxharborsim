use rumqttc::{AsyncClient, Event, EventLoop, Incoming, MqttOptions, QoS};
use std::env;
use std::time::Duration;

#[tokio::main]
async fn main() {
    let host = std::env::args()
        .nth(1)
        .unwrap_or_else(|| "localhost".into());
    let port: u16 = std::env::args()
        .nth(2)
        .map(|p| p.parse().unwrap_or(1883))
        .unwrap_or(1883);
    let topic = std::env::args()
        .nth(3)
        .unwrap_or_else(|| "test/topic".into());

    let mut mqttoptions = MqttOptions::new("rust-subscriber", host, port);
    mqttoptions.set_keep_alive(Duration::from_secs(60));

    if let (Ok(user), Ok(pass)) = (env::var("MQTT_USER"), env::var("MQTT_PASS")) {
        mqttoptions.set_credentials(user, pass);
    }

    let (client, mut eventloop) = AsyncClient::new(mqttoptions, 10);

    client.subscribe(&topic, QoS::AtMostOnce).await.unwrap();
    println!("Subscribed to {topic}, waiting for messages...");

    loop {
        match eventloop.poll().await {
            Ok(Event::Incoming(Incoming::Publish(p))) => {
                let payload = String::from_utf8_lossy(&p.payload);
                println!("Received on {}: {}", p.topic, payload);
            }
            Ok(_) => {} // ignore pings/acks
            Err(e) => {
                eprintln!("Error in event loop: {e}");
                break;
            }
        }
    }
}
