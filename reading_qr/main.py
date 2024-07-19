from qreader import QReader
import requests
import urllib.request
import json
import cv2

image = json.loads(requests.get('https://hackattic.com/challenges/reading_qr/problem?access_token=***REMOVED***').text)["image_url"]

print("Downloading from", image)
urllib.request.urlretrieve(image, "image.png")
print("Done! Detecting QR...")

qreader = QReader()
image = cv2.cvtColor(cv2.imread("image.png"), cv2.COLOR_BGR2RGB)
decoded_text = qreader.detect_and_decode(image=image)

print("Done! Code is", decoded_text[0])
print("Submitting...")
solution = json.loads(json.dumps({"code":decoded_text[0]}))
response = requests.post("https://hackattic.com/challenges/reading_qr/solve?access_token=***REMOVED***", json=solution)
print("Submitted! Response: ")
print(response.text)
