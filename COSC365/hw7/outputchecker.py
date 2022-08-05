import sys
import hashlib
BUF_SIZE = 10000  

md5 = hashlib.md5()

with open(sys.argv[1], 'rb') as f:
    while True:
        data = f.read(BUF_SIZE)
        if not data:
            break
        md5.update(data)

if ("ad9d4b305e78e41538eb9db4463d6de9" == md5.hexdigest()):
    print(sys.argv[1] + " matches the expected output.txt file.")
elif ("1eb9d9de6e17be14526e137df14c75ba" == md5.hexdigest()):
    print(sys.argv[1] + " matches sample.output.txt.")