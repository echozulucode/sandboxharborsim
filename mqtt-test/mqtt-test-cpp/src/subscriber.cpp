#include <mosquitto.h>
#include <csignal>
#include <cstring>
#include <iostream>
#include <string>

static bool running = true;
void handle_sigint(int) { running = false; }

static void on_connect(struct mosquitto* /*mosq*/, void* /*userdata*/, int rc) {
    if (rc == 0) {
        std::cout << "Connected to broker.\n";
    } else {
        std::cerr << "Connect failed, rc=" << rc << "\n";
    }
}

static void on_message(struct mosquitto* /*mosq*/, void* /*userdata*/,
                       const struct mosquitto_message* msg) {
    std::string payload;
    if (msg->payload && msg->payloadlen > 0) {
        payload.assign(static_cast<char*>(msg->payload), msg->payloadlen);
    }
    std::cout << "Received on " << msg->topic << ": " << payload << "\n";
}

int main(int argc, char** argv) {
    const char* host = (argc > 1) ? argv[1] : "localhost";
    int port = (argc > 2) ? std::stoi(argv[2]) : 1883;
    std::string topic = (argc > 3) ? argv[3] : "test/topic";

    std::signal(SIGINT, handle_sigint);
    std::signal(SIGTERM, handle_sigint);

    mosquitto_lib_init();

    mosquitto* client = mosquitto_new(nullptr, true, nullptr);
    if (!client) {
        std::cerr << "Failed to create mosquitto client\n";
        mosquitto_lib_cleanup();
        return 1;
    }

    const char* user = std::getenv("MQTT_USER");
    const char* pass = std::getenv("MQTT_PASS");
    if (user && pass) {
        if (mosquitto_username_pw_set(client, user, pass) != MOSQ_ERR_SUCCESS) {
            std::cerr << "Failed to set username/password\n";
        }
    }

    mosquitto_connect_callback_set(client, on_connect);
    mosquitto_message_callback_set(client, on_message);

    int rc = mosquitto_connect(client, host, port, /*keepalive*/60);
    if (rc != MOSQ_ERR_SUCCESS) {
        std::cerr << "Connect failed: " << mosquitto_strerror(rc) << "\n";
        mosquitto_destroy(client);
        mosquitto_lib_cleanup();
        return 1;
    }

    rc = mosquitto_subscribe(client, nullptr, topic.c_str(), /*qos*/0);
    if (rc != MOSQ_ERR_SUCCESS) {
        std::cerr << "Subscribe failed: " << mosquitto_strerror(rc) << "\n";
    } else {
        std::cout << "Subscribed to " << topic << ", waiting for messages...\n";
    }

    // Simple loop: handle callbacks until stopped
    while (running) {
        rc = mosquitto_loop(client, /*timeout_ms*/100, /*max_packets*/1);
        if (rc != MOSQ_ERR_SUCCESS) {
            std::cerr << "Loop error: " << mosquitto_strerror(rc) << "\n";
            break;
        }
    }

    mosquitto_disconnect(client);
    mosquitto_destroy(client);
    mosquitto_lib_cleanup();
    std::cout << "Subscriber exited cleanly.\n";
    return 0;
}
