package vrs_rpc

import (
	"log"

	"google.golang.org/grpc"
)

var (
	vrsConn   *grpc.ClientConn
	vrsClient VrsRpcClient

	faceConn   *grpc.ClientConn
	faceClient FaceRpcClient

	objectConn   *grpc.ClientConn
	objectClient ObjectRpcClient
)

//InitVrsClient 初始化相似检索rpc客户端
func InitVrsClient(address string) {
	var err error
	vrsConn, err = grpc.Dial(address, grpc.WithInsecure())
	if err != nil {
		log.Printf("连接检索服务器出错:%s\n", err)
	}
	vrsClient = NewVrsRpcClient(vrsConn)
	log.Printf("连接rpc服务器:%s\n", address)
}

//InitFaceClient 初始化人脸rpc客户端
func InitFaceClient(address string) {
	var err error
	faceConn, err = grpc.Dial(address, grpc.WithInsecure())
	if err != nil {
		log.Printf("连接人脸服务器出错:%s\n", err)
	}
	faceClient = NewFaceRpcClient(faceConn)
	log.Printf("连接人脸rpc服务器:%s\n", address)
}

//InitObjectClient 初始化物体检测rpc客户端
func InitObjectClient(address string) {
	var err error
	objectConn, err = grpc.Dial(address, grpc.WithInsecure())
	if err != nil {
		log.Printf("连接物体检测服务器出错:%s\n", err)
	}
	objectClient = NewObjectRpcClient(objectConn)
	log.Printf("连接物体检测rpc服务器:%s\n", address)
}

//CloseVrsClientConnect 关闭相似检索rpc客户端连接
func CloseVrsClientConnect() {
	if vrsConn != nil {
		vrsConn.Close()
		vrsConn = nil
	}
}

//CloseFaceClientConnect 关闭人脸rpc客户端连接
func CloseFaceClientConnect() {
	if faceConn != nil {
		faceConn.Close()
		faceConn = nil
	}
}

//CloseObjectClientConnect 关闭物体检测rpc客户端连接
func CloseObjectClientConnect() {
	if objectConn != nil {
		objectConn.Close()
		objectConn = nil
	}
}

//GetVrsClientInstance 获取相似检索rpc客户端实例
func GetVrsClientInstance() VrsRpcClient {
	return vrsClient
}

//GetFaceClientInstance 获取人脸rpc客户端实例
func GetFaceClientInstance() FaceRpcClient {
	return faceClient
}

//GetObjectClientInstance 获取物体检测rpc客户端实例
func GetObjectClientInstance() ObjectRpcClient {
	return objectClient
}
