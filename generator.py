import random
import string
import sys

z = 10
length = 10000
letters = string.ascii_lowercase

if len(sys.argv) > 1:
    z = int(sys.argv[1])

if len(sys.argv) > 2:
    length = int(sys.argv[2])

print(z, length)


for i in range(0, z):
    result_str1 = ''.join(random.choice(letters) for i in range(length))
    result_str2 = ''.join(random.choice(letters) for i in range(length))
    print(result_str1, result_str2)
