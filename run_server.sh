protoc --cpp_out=. --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` *.proto

g++ -o server -lprotobuf -labsl_synchronization -lgrpc -lgrpc++ server.cpp helloworld.pb.cc helloworld.grpc.pb.cc -std=c++17
g++ -o client -lprotobuf -labsl_synchronization -lgrpc -lgrpc++ client.cpp helloworld.pb.cc helloworld.grpc.pb.cc -std=c++17

./server 127.0.0.1:50002
