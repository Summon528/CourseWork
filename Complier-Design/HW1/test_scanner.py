import pytest
import os
import json
import requests
from bs4 import BeautifulSoup
import subprocess

ans_dir = os.path.join("test", "ans")
if not os.path.exists(ans_dir):
    os.makedirs(ans_dir)
ans_files_set = set(os.listdir(ans_dir))

in_dir = os.path.join("test", "in")
in_files = os.listdir(in_dir)


def get_ans():
    for file in in_files:
        if file not in ans_files_set:
            with open(os.path.join(in_dir, file), "r") as f:
                r = requests.post(
                    "https://sslab.cs.nctu.edu.tw/Compiler/project1",
                    {"code": f.read()})
                soup = BeautifulSoup(r.text, 'html.parser')
                ans = soup.find('code').text

                with open(os.path.join(ans_dir, file + "_raw"), "w") as f:
                    f.write(ans)

                with open(os.path.join(ans_dir, file), "w") as f:
                    ans_splited = ans.split('\n')
                    error_line = None
                    for idx, line in enumerate(ans_splited):
                        if line.startswith("Error at line"):
                            error_line = idx
                    if error_line is not None:
                        stderr = ans_splited[error_line]
                        ans_splited.pop(error_line)
                        f.write(
                            json.dumps({
                                "returncode": 1,
                                "stderr": stderr + '\n',
                                "stdout": '\n'.join(ans_splited)
                            }))
                    else:
                        f.write(
                            json.dumps({
                                "returncode": 0,
                                "stderr": "",
                                "stdout": ans
                            }))


get_ans()


@pytest.mark.parametrize("file", in_files)
def test_lex(file):
    r = subprocess.run(["./scanner", os.path.join(in_dir, file)],
                       capture_output=True)
    with open(os.path.join(ans_dir, file)) as f:
        j = json.loads(f.read())
        assert j["returncode"] == r.returncode
        assert j["stdout"] == r.stdout.decode()
        assert j["stderr"].split(":", 1)[0] == \
                r.stderr.decode().split(":", 1)[0]
