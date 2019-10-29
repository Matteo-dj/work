import binascii,sys,string

f = open(sys.argv[1],"rb")
ft = open(sys.argv[2],"wb") 
crc = binascii.crc32(f.read()) 
crc = hex(crc)
ft.write(crc.encode('utf-8'))
f.close()
ft.close