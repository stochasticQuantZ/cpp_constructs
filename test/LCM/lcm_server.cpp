#include <grpcpp/grpcpp.h>
#include "lcm.grpc.pb.h"
#include <iostream>
#include <numeric>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

class LCMServiceImpl final : public lcm::LCMService::Service {
  Status Compute(ServerContext* /*context*/, const lcm::LCMRequest* request,
                 lcm::LCMReply* reply) override {
    long long a = request->a();
    long long b = request->b();
    if (a == 0 || b == 0) {
      reply->set_result(0);
      return Status::OK;
    }
    long long g = std::gcd(std::llabs(a), std::llabs(b));
    long long l = (std::llabs(a) / g) * std::llabs(b);
    reply->set_result(l);
    return Status::OK;
  }
};

int main(int argc, char** argv) {
  std::string server_address("0.0.0.0:50051");
  LCMServiceImpl service;

  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "LCM server listening on " << server_address << std::endl;

  server->Wait();

  return 0;
}
