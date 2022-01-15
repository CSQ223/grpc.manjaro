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

class HelloWorldServiceImpl final:
    public HelloWorldService::Service
{
public:
    HelloWorldServiceImpl(const std::string& host): 
        host_(host)
    {
        std::cout << "[host] " << host_ << std::endl;
    }

    void RunServer()
    {
        using grpc::Server;
        using grpc::ServerBuilder;

        ServerBuilder builder;
        // Listen on the given address without any authentication mechanism.
        builder.AddListeningPort(host_, grpc::InsecureServerCredentials());
        // Register "service" as the instance through which we'll communicate with
        // clients. In this case it corresponds to an *synchronous* service.
        builder.RegisterService(this);
        // Finally assemble the server.
        std::unique_ptr<Server> server(builder.BuildAndStart());
        std::cout << "Server listening on " << host_ << std::endl;

        // Wait for the server to shutdown. Note that some other thread must be
        // responsible for shutting down the server for this call to ever return.
        server->Wait();
    }

    grpc::Status 
    SayHello(
        grpc::ServerContext* context, 
        const HelloRequest* request,
        HelloReply* reply
    ) override 
    {
        std::cout << "[gRPC] HelloWorldService::SayHello" << std::endl;
        reply->set_message("hello " + request->name());

        return grpc::Status::OK;
    }

private:
    std::string host_;
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

    HelloWorldServiceImpl server(argv[1]);
    server.RunServer();

    return 0;
}
