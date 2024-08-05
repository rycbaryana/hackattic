from sanic import Sanic, empty, json

import requests
import jwt
import json as j
from tools import tools

app = Sanic("JottingJWT")


# using ngrok to share my app with internet
url = "<MY NGROK DOMAIN>"
problem = "jotting_jwts"

@app.before_server_start
async def main(app, loop):
    print("Jotting JWT # rycbaryana")
    print("Gathering secret...")
    app.ctx.secret = tools.get_statement(problem)["jwt_secret"]
    app.ctx.res = ""
    print(f"Secret \"{app.ctx.secret}\" gathered!")


@app.post("/")
async def handle_jwt(request):
    token = request.body
    try:
        payload = jwt.decode(token, app.ctx.secret,
                            algorithms=["HS256"])
        if ("append" in payload):
            app.ctx.res += payload["append"]
            print(payload["append"], app.ctx.res)
        else:
            print("END!")
            return json({"solution":app.ctx.res})
    except:
        print("bad")
        pass
    return empty(status=200)
