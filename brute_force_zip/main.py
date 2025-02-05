from tools import tools
import zipfile
import os
import time

dir = os.path.dirname(__file__)
os.chdir(os.path.dirname(__file__))
problem = os.path.basename(dir)

zip_url = tools.get_statement(problem)["zip_url"]

tools.download_file(zip_url, "data.zip")

# Zip file contains The Dunwich Horror by Lovecraft https://github.com/keyunluo/pkcrack
import time

start_time = time.time()
newpid = os.fork()
if newpid == 0:
    binary = "/home/rycbaryana/pkcrack/bin/pkcrack"
    os.execvp(
        binary,
        [binary]
        + "-C data.zip -c dunwich_horror.txt -P attack.zip -p dunwich_horror.txt -d decrypted.zip -a".split(
            " "
        ),
    )
    os._exit(1)
os.wait()
print("--- %s seconds ---" % (time.time() - start_time))
with zipfile.ZipFile("decrypted.zip") as f:
    f.extract("secret.txt", dir)
with open("secret.txt") as f:
    secret = f.readline().strip()
    print(f"Secret is {secret}")
    tools.post_solution(problem, {"secret": secret})
