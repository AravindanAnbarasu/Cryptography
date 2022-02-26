import os

print "Special Soundness of Fiat-Shamir sigma-protocol.....Key extraction Started\n"
command = "g++ -o output fiat_shamir.cpp -lgmpxx -lgmp"
os.system(command)
os.system("./output")
