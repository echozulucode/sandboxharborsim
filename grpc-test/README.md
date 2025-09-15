# gRPC Demo

## Setup Quickstart
1. Install libraries `sudo apt-get install protobuf-compiler protobuf-compiler-grpc libprotobuf-dev libprotoc-dev libgrpc-dev libgrpc++-dev`
2. `uv init .` .. this is a rust based alternative to pip
3. `uv add  grpcio grpcio-tools`
4. `uv run -m grpc_tools.protoc -I proto --python_out=python --grpc_python_out=python proto/helloworld.proto`
5. `protoc -I proto --cpp_out=cpp --grpc_out=cpp --plugin=protoc-gen-grpc=$(which grpc_cpp_plugin) proto/helloworld.proto`


## Build C++
1. `cmake -S . -B build`
2. `cmake --build build -j`


```
mkdir -p build && cd build
cmake ..
cmake --build . -j

# Terminal A (C++ server):
./greeter_server

# Terminal B (C++ client):
./greeter_client
# Expected: C++ client got: Hello, Eric (from C++ server)!
```