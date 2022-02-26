import os

print "Decrypting CBC with simple XOR.....Started"
command = "g++ -o output cbc_xor.cpp "
os.system(command)
os.system("./output")
