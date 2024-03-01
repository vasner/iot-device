import os

from server import create_app
from server import configure_routes
from server import init_globals

app = create_app()

if __name__ == "__main__":
    server_host = os.getenv("IOT_SERVER_HOST") or "0.0.0.0"
    server_port = os.getenv("IOT_SERVER_PORT") or 8000
    device_port = os.getenv("IOT_DEVICE_PORT") or "/dev/ttyACM1"
    init_globals(device_port)
    configure_routes(app)
    app.run(host=server_host, port=server_port, threaded=True)
