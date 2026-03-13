#include <algorithm>
#include <iostream>
#include <set>
#include <string>

#include <grpcpp/grpcpp.h>
#include "permutation.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerWriter;
using grpc::Status;

using permutation::PermutationReply;
using permutation::PermutationRequest;
using permutation::PermutationService;

class PermutationServiceImpl final : public PermutationService::Service {
public:
    Status GetPermutations(ServerContext*,
                           const PermutationRequest* request,
                           ServerWriter<PermutationReply>* writer) override {
        std::string s = request->input();
        std::sort(s.begin(), s.end());

        std::set<std::string> seen;

        do {
            if (seen.insert(s).second) {
                PermutationReply reply;
                reply.set_value(s);
                if (!writer->Write(reply)) {
                    break;
                }
            }
        } while (std::next_permutation(s.begin(), s.end()));

        return Status::OK;
    }
};

int main() {
    const std::string address = "0.0.0.0:50051";
    PermutationServiceImpl service;

    ServerBuilder builder;
    builder.AddListeningPort(address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    auto server = builder.BuildAndStart();
    std::cout << "Server listening on " << address << '\n';
    server->Wait();
    return 0;
}