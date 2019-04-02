import re
import sys
prompt = "Enter the {} ball (x,y,range), enter “exit” to quit the program: "
x, y, r = [0]*2, [0]*2, [0]*2
for idx, txt in enumerate(["first", "second"]):
    while True:  # loop until valid input
        data = input(prompt.format(txt))
        # if the input match any kind of ExIt, exit the program
        if re.match(r"[Ee][Xx][Ii][Tt]", data):
            print("Thank you. Byebye!")
            sys.exit(0)
        # Use regex to find numbers in the input string
        match = re.match(r"\( *([\d]+) *, *([\d]+) *, *([\d]+) *\)", data)
        # If the input doesn't match, input again
        if match is None:
            print("Bad input!")
            continue
        x[idx], y[idx], r[idx] = map(int, match.groups())
        break

# caculate if the two balls collide
print("The two balls are not colliding!"
      if (x[0]-x[1])**2+(y[0]-y[1])**2 > (r[0]+r[1])**2
      else "The two balls are colliding!")
