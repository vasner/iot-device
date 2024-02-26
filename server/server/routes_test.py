import json
import pytest

from server import create_app
from server import configure_routes


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
    response = client.get("/api/info")
    assert response.status_code == 200
    info = json.loads(response.data)
    assert "name" in info
    assert "version" in info
