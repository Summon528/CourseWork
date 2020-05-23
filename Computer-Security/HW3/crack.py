
x = open("./task1_result.log","r")
ciph = list(x.read())
print(ciph)
# for key in range(257):
key = 133
s = ""
print(key)
for i in ciph:
    s += chr(ord(i) ^ key)
print(s)
print(list(s))



# x = open("./out.txt","r")
# ciph = list(x.read())
# # print(ciph)
# # for key in range(257):
# key = 133
# s = ""
# # print(key)
# for i in ciph:
#     s += chr(ord(i) ^ key)

# with open("task1_result.log", "w") as f:
#     f.write(s)
# # print(list(s))
