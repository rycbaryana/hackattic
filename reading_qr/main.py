from qreader import QReader
import cv2
import os
from tools import tools

if __name__ == "__main__":
    file = os.path.join(os.path.dirname(__file__), "image.png")
    problem = "reading_qr"
    image = tools.get_statement(problem)["image_url"]

    tools.download_file(image, file)

    qreader = QReader()
    image = cv2.cvtColor(cv2.imread(file), cv2.COLOR_BGR2RGB)
    decoded_text = qreader.detect_and_decode(image=image)

    print("Done! Code is", decoded_text[0])
    print(tools.post_solution(problem, {"code": decoded_text[0]}, True))
