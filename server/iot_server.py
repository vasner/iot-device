import os

from server import create_app
from server import configure_routes

app = create_app()

if __name__ == "__main__":
    host = os.getenv("IOT_SERVER_HOST") or "0.0.0.0"
    port = os.getenv("IOT_SERVER_PORT") or 8000
    configure_routes(app)
    app.run(host=host, port=port, threaded=True)
