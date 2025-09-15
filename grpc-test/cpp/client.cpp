#include <grpcpp/grpcpp.h>
#include <iostream>
#include "helloworld.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using helloworld::Greeter;
using helloworld::HelloReply;
using helloworld::HelloRequest;

int main() {
  auto channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
  std::unique_ptr<Greeter::Stub> stub = Greeter::NewStub(channel);

  HelloRequest req;
  req.set_name("Eric");
  HelloReply reply;
  ClientContext ctx;

  Status status = stub->SayHello(&ctx, req, &reply);
  if (!status.ok()) {
    std::cerr << "RPC failed: " << status.error_code() << " - " << status.error_message() << "\n";
    return 1;
  }

  std::cout << "C++ client got: " << reply.message() << std::endl;
  return 0;
}
