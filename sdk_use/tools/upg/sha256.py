#! python
import hashlib,sys

f = open(sys.argv[1],"rb")
ft = open(sys.argv[2],"wb")
sh = hashlib.sha256()
sh.update(f.read())
ft.write(sh.hexdigest().encode('utf-8'))
f.close()
ft.close
