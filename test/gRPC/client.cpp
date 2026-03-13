#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "permutation.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::Status;

using permutation::PermutationReply;
using permutation::PermutationRequest;
using permutation::PermutationService;

class PermutationClient {
public:
    explicit PermutationClient(std::shared_ptr<Channel> channel)
        : stub_(PermutationService::NewStub(channel)) {}

    void GetPermutations(const std::string& input) {
        PermutationRequest request;
        request.set_input(input);

        ClientContext context;
        std::unique_ptr<ClientReader<PermutationReply>> reader(
            stub_->GetPermutations(&context, request));

        PermutationReply reply;
        while (reader->Read(&reply)) {
            std::cout << '"' << reply.value() << '"' << '\n';
        }

        Status status = reader->Finish();
        if (!status.ok()) {
            std::cerr << "RPC failed: " << status.error_message() << '\n';
        }
    }

private:
    std::unique_ptr<PermutationService::Stub> stub_;
};

int main(int argc, char** argv) {
    std::string input = "abc";
    if (argc > 1) {
        input = argv[1];
    }

    PermutationClient client(
        grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));

    client.GetPermutations(input);
    return 0;
}