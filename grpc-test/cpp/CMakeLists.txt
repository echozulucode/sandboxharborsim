cmake_minimum_required(VERSION 3.16)
project(grpc_helloworld_cpp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(Protobuf REQUIRED)
find_package(gRPC REQUIRED)

# Generated sources from protoc (already produced in step 3b)
set(PROTO_SRCS helloworld.pb.cc helloworld.grpc.pb.cc)
set(PROTO_HDRS helloworld.pb.h  helloworld.grpc.pb.h)

add_executable(greeter_server server.cpp ${PROTO_SRCS})
target_include_directories(greeter_server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(greeter_server PRIVATE gRPC::grpc++ protobuf::libprotobuf)

add_executable(greeter_client client.cpp ${PROTO_SRCS})
target_include_directories(greeter_client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(greeter_client PRIVATE gRPC::grpc++ protobuf::libprotobuf)
