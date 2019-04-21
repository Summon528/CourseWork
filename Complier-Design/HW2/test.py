import os
import json
import subprocess
import re

ans_dir = os.path.join("test", "ans")
if not os.path.exists(ans_dir):
    os.makedirs(ans_dir)
ans_files = os.listdir(ans_dir)

in_dir = os.path.join("test", "in")
in_files = os.listdir(in_dir)
in_files.sort(
    key=lambda x: int(x.split(".")[0]) if x.split(".")[0].isdigit() else -1)

out_dir = os.path.join("test", "out")
if not os.path.exists(out_dir):
    os.makedirs(out_dir)


def get_ans():
    from bs4 import BeautifulSoup
    import requests
    for file in set(in_files) - set(ans_files):
        with open(os.path.join(in_dir, file), "r") as f:
            r = requests.post("https://sslab.cs.nctu.edu.tw/Compiler/project2",
                              {"code": f.read()})
            soup = BeautifulSoup(r.text, 'html.parser')
            ans = soup.find('code').text

            with open(os.path.join(ans_dir, file), "w") as f:
                f.write(ans)


def check_ans(ans_str, out_str, file_name):
    ans_match = re.search(r"Error found in Line #(\d*)", ans_str)
    out_match = re.search(r"Error found in Line #(\d*)", out_str)
    ans_error_line = ans_match.group(1) if ans_match else -1
    out_error_line = out_match.group(1) if out_match else -1
    print(file_name, end="\t")
    if ans_error_line == out_error_line:
        print("\033[92mAC\033[0m")
        return True
    else:
        print(f"\033[91mWA\033[0m\tans:{ans_error_line} out:{out_error_line}")
        return False


def test():
    r = subprocess.run(["make"])
    ac, wa = 0, 0
    for file in in_files:
        r = subprocess.run(["./parser", os.path.join(in_dir, file)],
                           capture_output=True)
        out_str = r.stdout.decode() + r.stderr.decode()
        with open(os.path.join(out_dir, file), "w") as f:
            f.write(out_str)
        with open(os.path.join(ans_dir, file)) as f:
            if check_ans(f.read(), out_str, file):
                ac += 1
            else:
                wa += 1
    print(f"Test complete {ac}/{ac+wa}")


get_ans()
test()
