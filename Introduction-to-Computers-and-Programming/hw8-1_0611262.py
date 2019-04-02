from statistics import mean
# Read all lines from file
# filter out line that starts with X-DSPAM-Confidence
# convert to float and calc mean
print("Average spam confidence: {}".
      format(mean([float(i[i.find(":")+1:])
                   for i in open(input("Enter the file name: ")).readlines()
                   if i.startswith("X-DSPAM-Confidence:")])))
