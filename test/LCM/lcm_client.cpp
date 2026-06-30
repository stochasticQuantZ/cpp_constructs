#include <grpcpp/grpcpp.h>
#include "lcm.grpc.pb.h"
#include <iostream>

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

int main(int argc, char** argv) {
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " <a> <b>" << std::endl;
    return 1;
  }

  long long a = std::stoll(argv[1]);
  long long b = std::stoll(argv[2]);

  auto channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
  std::unique_ptr<lcm::LCMService::Stub> stub = lcm::LCMService::NewStub(channel);

  lcm::LCMRequest req;
  req.set_a(a);
  req.set_b(b);

  lcm::LCMReply reply;
  ClientContext ctx;
  Status status = stub->Compute(&ctx, req, &reply);

  if (!status.ok()) {
    std::cerr << "RPC failed: " << status.error_message() << std::endl;
    return 1;
  }

  std::cout << "LCM(" << a << ", " << b << ") = " << reply.result() << std::endl;
  return 0;
}
