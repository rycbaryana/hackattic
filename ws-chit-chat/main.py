import websocket
from time import time
import tools

intervals = [700, 1500, 2000, 2500, 3000]


def round_interval(interval):
    return min(intervals, key=lambda x: abs(x - interval))


def on_open(ws):
    global timer
    print("Opened connection!")
    timer = time()


def on_message(ws, message):
    global timer
    print(message)
    if message == "ping!":
        cur = time()
        delta = (cur - timer) * 1000
        rounded = round_interval(delta)
        print(f"Interval is {delta}. Rounded interval is {rounded}")
        ws.send(str(rounded))
        timer = cur
    if message.startswith("congratulations"):
        solution = message.split('"')[1]
        print(f"Got solution {solution}")
        tools.post_solution("websocket_chit_chat", {"secret": solution}, True)


def on_close(ws):
    print("Closed!")


def main():
    statement = tools.get_statement("websocket_chit_chat")

    token = statement["token"]
    print(f"Got token ${token}")

    ws = websocket.WebSocketApp(
        url=f"wss://hackattic.com/_/ws/{token}",
        on_open=on_open,
        on_message=on_message,
        on_close=on_close,
    )
    ws.run_forever()


if __name__ == "__main__":
    main()
