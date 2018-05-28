#！/bin/bash
echo "Building gRPC protos"

protoc -I protos/ --grpc_out=./base_engine/src/rpc --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` protos/vrs.proto
protoc -I protos/ --cpp_out=./base_engine/src/rpc protos/vrs.proto
echo "CPP verson done"

python3 -m grpc_tools.protoc -I./protos --python_out=./deep_service/ --grpc_python_out=./deep_service/ protos/vrs.proto 
echo "Python version done"

protoc --go_out=plugins=grpc:./web_server/server/src/vrs_rpc protos/vrs.proto
echo "Golang version done"