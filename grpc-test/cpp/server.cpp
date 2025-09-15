#include <grpcpp/grpcpp.h>
#include "helloworld.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using helloworld::Greeter;
using helloworld::HelloReply;
using helloworld::HelloRequest;

class GreeterServiceImpl final : public Greeter::Service {
public:
  Status SayHello(ServerContext* context, const HelloRequest* req, HelloReply* reply) override {
    reply->set_message("Hello, " + req->name() + " (from C++ server)!");
    return Status::OK;
  }
};

int main() {
  std::string server_address("0.0.0.0:50051");
  GreeterServiceImpl service;

  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);

  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "C++ gRPC server listening on " << server_address << std::endl;
  server->Wait();
  return 0;
}
