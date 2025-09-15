#include <mosquitto.h>
#include <chrono>
#include <csignal>
#include <cstring>
#include <iostream>
#include <string>
#include <thread>

static bool running = true;
void handle_sigint(int) { running = false; }

int main(int argc, char** argv) {
    const char* host = (argc > 1) ? argv[1] : "localhost";
    int port = (argc > 2) ? std::stoi(argv[2]) : 1883;
    std::string topic = (argc > 3) ? argv[3] : "test/topic";

    std::signal(SIGINT, handle_sigint);
    std::signal(SIGTERM, handle_sigint);

    mosquitto_lib_init();

    // client id "", clean session true
    mosquitto* client = mosquitto_new(nullptr, true, nullptr);
    if (!client) {
        std::cerr << "Failed to create mosquitto client\n";
        mosquitto_lib_cleanup();
        return 1;
    }

    // Optional: set username/password via env vars
    const char* user = std::getenv("MQTT_USER");
    const char* pass = std::getenv("MQTT_PASS");
    if (user && pass) {
        if (mosquitto_username_pw_set(client, user, pass) != MOSQ_ERR_SUCCESS) {
            std::cerr << "Failed to set username/password\n";
        }
    }

    // Connect
    int rc = mosquitto_connect(client, host, port, /*keepalive*/60);
    if (rc != MOSQ_ERR_SUCCESS) {
        std::cerr << "Connect failed: " << mosquitto_strerror(rc) << "\n";
        mosquitto_destroy(client);
        mosquitto_lib_cleanup();
        return 1;
    }

    int count = 0;
    while (running) {
        std::string payload = "Hello MQTT " + std::to_string(count++);
        rc = mosquitto_publish(client, nullptr, topic.c_str(),
                               static_cast<int>(payload.size()),
                               payload.data(),
                               /*qos*/0, /*retain*/false);
        if (rc != MOSQ_ERR_SUCCESS) {
            std::cerr << "Publish failed: " << mosquitto_strerror(rc) << "\n";
        } else {
            std::cout << "Published: " << payload << "\n";
        }

        // Process network traffic (non-blocking)
        mosquitto_loop(client, /*timeout_ms*/10, /*max_packets*/1);
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    mosquitto_disconnect(client);
    mosquitto_destroy(client);
    mosquitto_lib_cleanup();
    std::cout << "Publisher exited cleanly.\n";
    return 0;
}
