import os
import sqlite3


class RCLoader:
    @staticmethod
    def Package(resfolder):
        if os.path.exists('res.db'):
            os.remove('res.db')
        db = sqlite3.connect('res.db')
        db.execute('''CREATE TABLE "res" (
        "id"  TEXT NOT NULL,
        "bin"  BLOB NOT NULL,PRIMARY KEY ("id"));''')
        for root, dirs, files in os.walk(resfolder):
            for f in files:
                f = os.path.join(root, f)
                rid = f.replace(resfolder, '').lstrip('\\').replace('\\', '.')
                with open(f, 'rb') as input_file:
                    value = input_file.read()
                    db.execute('INSERT INTO res VALUES(?,?)', [rid, sqlite3.Binary(value)])
                    #dll.write(value)
        db.commit()
        db.close()

    @staticmethod
    def Load(imageid):
        if not imageid:
            return
        db = sqlite3.connect('res.db')
        cursor = db.cursor()
        cursor.execute("SELECT bin FROM res WHERE id = ?", (imageid,))
        buf = cursor.fetchone()
        cursor.close()
        db.close()
        return buf[0]


    ########################################################################
    @staticmethod
    def GetCurrentPath():
        import os, sys, inspect

        path = os.path.realpath(sys.path[0])        # interpreter starter's path
        if os.path.isfile(path):                    # starter is excutable file
            path = os.path.dirname(path)
            return os.path.abspath(path)            # return excutable file's directory
        else:                                       # starter is python script
            caller_file = inspect.stack()[1][1]     # function caller's filename
            # return function caller's file's directory
            return os.path.abspath(os.path.dirname(caller_file))



