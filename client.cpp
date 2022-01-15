#include <grpcpp/grpcpp.h>
#include "helloworld.grpc.pb.h"
#include "helloworld.pb.h"

#include <grpcpp/support/status.h>
#include <grpcpp/impl/codegen/core_codegen_interface.h>
#include <grpcpp/impl/codegen/grpc_library.h>
#include <grpcpp/impl/grpc_library.h>

grpc::CoreCodegenInterface* grpc::g_core_codegen_interface 	= nullptr;
grpc::GrpcLibraryInterface* grpc::g_glip 					= nullptr;

static ::grpc::internal::GrpcLibraryInitializer g_gli_initializer;

namespace grpc {

 const Status& Status::OK = Status();
 const Status& Status::CANCELLED =
    Status(StatusCode::CANCELLED, "");

}  // namespace grpc

using namespace rpc_package;


class HelloWorldClient
{
public:
    HelloWorldClient(const std::string& host):
        host_( host )
    {
        stub_ = HelloWorldService::NewStub(grpc::CreateChannel(
            host_,
            grpc::InsecureChannelCredentials()
        ));
    }

    std::string CallSayHello(const std::string& name)
    {
        HelloRequest req;
        req.set_name(name);

        HelloReply reply;
        grpc::ClientContext ctx;

        grpc::Status status = stub_->SayHello(
            &ctx,
            req,
            &reply
        );

        if (status.ok())
        {
            std::cout << reply.message() << std::endl;
            return reply.message();
        }
        else
        {
            std::cout << status.error_code() << ":" 
                      << status.error_message() << std::endl;
            return "failed";
        }
    }


private:
    std::string host_;
    std::unique_ptr<HelloWorldService::Stub> stub_;
};


int
main(int argc, char **argv)
{
    if (argc<2)
    {
        std::cout << "[Usage]: " << argv[0] << " ip:port" << std::endl;
        std::cout << "         " << argv[0] << " 127.0.0.1:50002" << std::endl;
        return 1;
    }

    HelloWorldClient client(argv[1]);
    std::cout << client.CallSayHello("xuwei") << std::endl;

    return 0;
}