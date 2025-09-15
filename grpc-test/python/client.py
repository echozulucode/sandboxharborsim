import grpc
import helloworld_pb2 as pb2
import helloworld_pb2_grpc as pb2_grpc

def main():
    with grpc.insecure_channel("localhost:50051") as channel:
        stub = pb2_grpc.GreeterStub(channel)
        resp = stub.SayHello(pb2.HelloRequest(name="Eric"))
        print("Python client got:", resp.message)

if __name__ == "__main__":
    main()
