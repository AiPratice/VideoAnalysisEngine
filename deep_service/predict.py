import math
from sklearn import neighbors
import os
import os.path
import pickle
from PIL import Image, ImageDraw
import face_recognition
from face_recognition.face_recognition_cli import image_files_in_folder
import psycopg2
import face_db

ALLOWED_EXTENSIONS = {'png', 'jpg', 'jpeg'}

#根据图片路径预测其中的人物
def predict(X_img_path, knn_clf=None, model_path=None, distance_threshold=0.6):
    if not os.path.isfile(X_img_path) or os.path.splitext(X_img_path)[1][1:] not in ALLOWED_EXTENSIONS:
        raise Exception("错误的输入图像路径:{}".format(X_img_path))

    if knn_clf is None and model_path is None:
        raise Exception("knn分类模型文件输入为空")

    if knn_clf is None:
        with open(model_path, 'rb') as f:
            #模型反反序列化
            knn_clf = pickle.load(f)

    #从输入图片中进行人脸信息的定位
    X_img = face_recognition.load_image_file(X_img_path)
    X_face_locations = face_recognition.face_locations(X_img)

    #找不到人脸定位信息，返回空数组
    if len(X_face_locations) == 0:
        return []

    #对输入图像的人脸进行编码
    faces_encodings = face_recognition.face_encodings(X_img, known_face_locations=X_face_locations)

    #使用knn模型去找出输入图像里人脸的最佳匹配
    closet_distances = knn_clf.kneighbors(faces_encodings, n_neighbors=1)
    are_matches = [closet_distances[0][i][0] <= distance_threshold for i in range(len(X_face_locations))]
    
    return [(pred, loc) if rec else (0, loc) for pred, loc, rec in zip(knn_clf.predict(faces_encodings), X_face_locations, are_matches)]

if __name__ == "__main__":
    knn_clf = None
    with open('trained_knn_model.clf', 'rb') as f:
        knn_clf = pickle.load(f)

    for img_path in os.listdir('test/'):
        path = os.path.join('test/', img_path)

        print('从文件 %s 中寻找人脸' % path)
        predictions = predict(path,knn_clf=knn_clf)

        for person_id, (top, right, bottom, left) in predictions:
            print("- Found {} at ({}, {})".format(person_id, left, top))
