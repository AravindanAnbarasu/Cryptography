import os
command = "g++ -o cryptoLib cryptoLib_test.cpp"
os.system(command)
os.system("./cryptoLib")
