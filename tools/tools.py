import requests
import urllib
import tools
import os

dir = os.path.dirname(__file__)
with open(os.path.join(dir, "token.txt")) as f:
    token = f.readline()
    print(f"Token found {token}")

def get_statement(problem: str):
    url = "https://hackattic.com/challenges/" + \
        problem + "/problem?access_token=" + token
    print(f"Getting statement for {problem} problem...")
    resp = requests.get(url).json()
    print(resp)
    return resp


def post_solution(problem: str, solution, playground=False):
    url = "https://hackattic.com/challenges/" + \
        problem + "/solve?access_token=" + token
    if (playground):
        url += "&playground=1"
    print(f"Posting solution for {problem} problem...")
    resp = requests.post(url, json=solution).json()
    print(resp)
    return resp
def download_file(url, filename):
    print(f"Downloading from {url} to {filename}...")
    urllib.request.urlretrieve(url, filename)
    print(f"Downloaded!")
    
