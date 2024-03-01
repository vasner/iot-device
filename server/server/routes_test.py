import os
import json
import pytest

from server import create_app
from server import configure_routes
from server import init_globals, deinit_globals


@pytest.fixture()
def app():
    app = create_app()
    configure_routes(app)
    app.config.update({"TESTING": True})
    yield app


@pytest.fixture()
def client(app):
    return app.test_client()


def test_info(client):
    device_port = os.getenv("IOT_DEVICE_PORT") or "/dev/ttyACM1"
    try:
        init_globals(device_port)
    except Exception:
        pytest.skip("Device is not connected or failed to initialize")
    else:
        response = client.get("/api/info")
        deinit_globals()
        assert response.status_code == 200
        info = json.loads(response.data)
        assert "name" in info
        assert "server_version" in info
        assert "device_version" in info


def test_sample(client):
    device_port = os.getenv("IOT_DEVICE_PORT") or "/dev/ttyACM1"
    try:
        init_globals(device_port)
    except Exception:
        pytest.skip("Device is not connected or failed to initialize")
    else:
        response = client.get("/api/sample")
        deinit_globals()
        assert response.status_code == 200
        sample = json.loads(response.data)
        assert "controls" in sample
        assert "sensors" in sample
