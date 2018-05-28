import psycopg2
import threading

_conn = None
_mutex = threading.Lock()
def getConnInstance():
    global _conn
    global _mutex
    if _conn == None:
        _mutex.acquire()
        if _conn == None:
            _conn = psycopg2.connect(database='vrs_db', user='postgres', password='postgres', host='127.0.0.1', port='5432')
        _mutex.release()
    return _conn