import re
# Read all lines from file
# filter out line that starts with "Subject: [sakai] svn commit"
# use regex to search the line
# print the required match groups
data = [re.search(r"commit: ([^-]*) -( in)? ([^\/]*)", i)
        for i in open(input("Enter the file name: ")).readlines()
        if i.startswith("Subject: [sakai] svn commit")]
for i in data:
    print(i.group(1), i.group(3))
