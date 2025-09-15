use rumqttc::{AsyncClient, MqttOptions, QoS};
use std::env;
use std::time::Duration;
use tokio::time;

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

    let mut mqttoptions = MqttOptions::new("rust-publisher", host, port);
    mqttoptions.set_keep_alive(Duration::from_secs(60));

    if let (Ok(user), Ok(pass)) = (env::var("MQTT_USER"), env::var("MQTT_PASS")) {
        mqttoptions.set_credentials(user, pass);
    }

    let (client, mut eventloop) = AsyncClient::new(mqttoptions, 10);

    // Spawn a task to handle network traffic
    tokio::spawn(async move {
        loop {
            let _ = eventloop.poll().await;
        }
    });

    let mut count = 0u64;
    loop {
        let payload = format!("Hello MQTT {count}");
        if let Err(e) = client
            .publish(&topic, QoS::AtMostOnce, false, payload.clone())
            .await
        {
            eprintln!("Publish error: {e}");
        } else {
            println!("Published: {payload}");
        }
        count += 1;
        time::sleep(Duration::from_secs(2)).await;
    }
}
