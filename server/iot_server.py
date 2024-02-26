import json
from flask import Flask, Response

app = Flask(__name__)


@app.route("/info", methods=["GET"])
def info():
    # TODO: Get from .toml file
    return Response(json.dumps({"name": "iot-device server", "version": "0.1.0"}))


if __name__ == "__main__":
    # TODO: Get following parameters from environment variables
    app.run(host="0.0.0.0", port=8000, threaded=True)
