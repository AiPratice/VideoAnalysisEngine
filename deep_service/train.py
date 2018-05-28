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

#训练人脸分类器
def trainClassifer(model_save_path = None, n_neighbors = None, knn_algo = 'ball_tree', verbose = False):
    X = []
    y = []
    conn = face_db.getConnInstance()
    cur = conn.cursor()
    cur.execute('SELECT * FROM persons')
    for record in cur:
        personID = record[0]
        name = record[1]
        print("personID:%s,name:%s" % (personID,name))

        cur2 = conn.cursor()
        cur2.execute('SELECT path FROM person_faces WHERE person_id = %s',(personID,))
        for record2 in cur2:
            path = record2[0]
            image = face_recognition.load_image_file(path)
            face_bounding_boxes = face_recognition.face_locations(image)
            print('path:%s' % path)
            print(face_bounding_boxes)

            if len(face_bounding_boxes) != 1:
                print('【警告】图像:%s 所包含的人脸数应为一个' % path)
                continue
            
            X.append(face_recognition.face_encodings(image,known_face_locations=face_bounding_boxes)[0])
            y.append(personID)

    if n_neighbors is None:
        n_neighbors = int(round(math.sqrt(len(X))))
    print('n_neighbors:%d' % n_neighbors)

    #创建并训练knn分类器
    knn_clf = neighbors.KNeighborsClassifier(n_neighbors=n_neighbors, algorithm=knn_algo, weights='distance')
    knn_clf.fit(X, y)

    #保存训练得到的knn分类器
    # Save the trained KNN classifier
    if model_save_path is not None:
        with open(model_save_path, 'wb') as f:
            print('save model:'+model_save_path)
            pickle.dump(knn_clf, f)

    return knn_clf
    
#加载knn人脸模型
def loadKnnClfModel(path):
    knn_clf = None
    with open(path, 'rb') as f:
        #模型反反序列化
        knn_clf = pickle.load(f)
    return knn_clf