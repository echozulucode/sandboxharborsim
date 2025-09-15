import grpc
from concurrent import futures
import time

import helloworld_pb2 as pb2
import helloworld_pb2_grpc as pb2_grpc

class Greeter(pb2_grpc.GreeterServicer):
    def SayHello(self, request, context):
        return pb2.HelloReply(message=f"Hello, {request.name} (from Python server)!")

def serve():
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=4))
    pb2_grpc.add_GreeterServicer_to_server(Greeter(), server)
    server.add_insecure_port("[::]:50051")
    server.start()
    print("Python gRPC server listening on 0.0.0.0:50051")
    try:
        while True:
            time.sleep(86400)
    except KeyboardInterrupt:
        server.stop(grace=None)

if __name__ == "__main__":
    serve()
