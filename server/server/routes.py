import json
import toml

from flask import Response


def configure_routes(app):
    @app.route("/api/info", methods=["GET"])
    def info():
        with open("pyproject.toml", "r") as file:
            project = toml.loads(file.read().rstrip())
        return Response(
            json.dumps({"name": project["tool"]["poetry"]["name"], "version": project["tool"]["poetry"]["version"]}),
            mimetype="application/json",
        )

    @app.route("/api/sample", methods=["GET"])
    def sample():
        pass
