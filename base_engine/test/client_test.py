# -*- coding:utf-8 -*-
import socket
import sys

if __name__ == "__main__":
    SOCKET_FILE_PATH = "/tmp/vrs_server.socket"
    sock = socket.socket(socket.AF_UNIX,socket.SOCK_STREAM)
    print("连接到socket:%s" % SOCKET_FILE_PATH)
    try:
        sock.connect(SOCKET_FILE_PATH)
    except Exception as e:
        print(e)
        sys.exit(-1)
    try:
        message = "message test"
        sock.sendall(message.encode('utf_8'))

        amount_received = 0
        amount_expected = len(message)

        while amount_received < amount_expected:
            data = sock.recv(16)
            amount_received += len(data)
            print('received "%s"' % data)
    finally:
        print("关闭socket")
        sock.close()
