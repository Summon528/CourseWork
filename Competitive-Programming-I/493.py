input()
print 'lose' if reduce(lambda x, y: x ^ int(y).bit_length(), raw_input().split(), 0) else 'win'
