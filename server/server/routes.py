import json
import toml

from flask import Response
from .iot_device import IotDevice

device: IotDevice = None


def configure_routes(app):
    @app.route("/api/info", methods=["GET"])
    def info():
        global device
        with open("pyproject.toml", "r") as file:
            project = toml.loads(file.read().rstrip())
        return Response(
            json.dumps(
                {
                    "name": project["tool"]["poetry"]["name"],
                    "server_version": project["tool"]["poetry"]["version"],
                    "device_version": device.version,
                }
            ),
            mimetype="application/json",
        )

    @app.route("/api/sample", methods=["GET"])
    def sample():
        global device
        return Response(device.get_sample_json(), mimetype="application/json")


def init_globals(device_port: str):
    global device
    device = IotDevice(port=device_port)


def deinit_globals():
    global device
    device.close()
