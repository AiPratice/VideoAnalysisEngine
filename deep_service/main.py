"""
人脸检索knn训练
"""
from concurrent import futures
import grpc
import time
import math
import vrs_pb2_grpc
import vrs_pb2
import yaml
import sys
import os.path
import face_recognition
import lightnet

import config
import train
import predict

#人脸检测实现
class FaceRpcServicer(vrs_pb2_grpc.FaceRpcServicer):
    def __init__(self):
        #加载人脸knn模型
        self.knn_model_path = 'models/trained_knn_model.clf'
        if os.path.exists(self.knn_model_path):
            self.knn_clf = train.loadKnnClfModel(self.knn_model_path)
 
    #建立分类器
    def BuildClassifer(self, request, context):
        print('BuildClassifer')
        self.knn_clf = train.trainClassifer(model_save_path=self.knn_model_path, n_neighbors=2)
        return vrs_pb2.Result(
            Code = 200, 
            Msg = ''
        )

    #在指定的视频中获取人物
    def DetectPersonsInVideo(self, request, context):
        print(request)
        return vrs_pb2.Result(
            Code = 200,
            Msg = ''
        )
    
    #从图片中探测人脸
    def DetectPersonInImg(self, request, context):
        print(request)
        predictions = predict.predict(X_img_path=request.Path,knn_clf=self.knn_clf,distance_threshold=0.61)
        print(predictions)
        for person_id, (top, right, bottom, left) in predictions:  
            yield vrs_pb2.DetectPersonInImgResult( 
                PersonID = person_id,
                Top = top,
                Right = right,
                Bottom = bottom,
                Left = left
            )

#物体检测实现
class ObjectRpcServicer(vrs_pb2_grpc.ObjectRpcServicer):
    def __init__(self):
        self.model = lightnet.load('yolo')

    #图片物体检测
    def DetectObjectInImg(self, request, context):
        print(request)
        image = lightnet.Image.from_bytes(open(request.Path, 'rb').read())
        boxes = self.model(image)
        print(boxes)
        for class_id, class_name, prob, (x, y, width, height) in boxes:
            yield vrs_pb2.DetectObjectInImgResult(
                ClassID = class_id,
                ClassName = class_name,
                Prob = prob,
                Left = int(round(x - width/2)),
                Top = int(round(y - height/2)),
                Width = int(round(width)),
                Height = int(round(height))
            )

def serve():
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=2))
    vrs_pb2_grpc.add_FaceRpcServicer_to_server(
        FaceRpcServicer(), server)
    vrs_pb2_grpc.add_ObjectRpcServicer_to_server(
        ObjectRpcServicer(), server)
    server.add_insecure_port('127.0.0.1:50052')
    server.start()

    try:
        while True:
            time.sleep(60)
    except KeyboardInterrupt:
        server.stop(0)

if __name__ == '__main__':
    #config_instance = config.Config('../config.yaml')
    #print(config_instance.get_yaml_config())
    serve()