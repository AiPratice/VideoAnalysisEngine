#！/bin/bash
echo "构建proto"
protoc --go_out=plugins=grpc:src/vrs_rpc protos/vrs.proto