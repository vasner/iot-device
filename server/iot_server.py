import os
import json
import toml
from flask import Flask, Response

app = Flask(__name__)


@app.route("/info", methods=["GET"])
def info():
    with open("pyproject.toml", "r") as file:
        project = toml.loads(file.read().rstrip())
    return Response(
        json.dumps({"name": project["tool"]["poetry"]["name"], "version": project["tool"]["poetry"]["version"]})
    )


if __name__ == "__main__":
    host = os.getenv("IOT_SERVER_HOST") or "0.0.0.0"
    port = os.getenv("IOT_SERVER_PORT") or 8000
    app.run(host=host, port=port, threaded=True)
