# iot-device server

## Project structure

* [server] - Server components source code
* [web_root][web_root] - Web-server source files

## Environment

### Dependency manager

As packaging and dependency management system use [Poetry](https://python-poetry.org/).

To force Poetry store environment inside server's directory run command:
```shell
poetry config virtualenvs.in-project true
```

### Install

```shell
poetry install
```

## Run server

Set environment variables:

| Variable        | Description                          |
|-----------------|--------------------------------------|
| IOT_SERVER_HOST | Server host IP, default is "0.0.0.0" |
| IOT_SERVER_PORT | Server port, default is 8000         |

```shell
export IOT_SERVER_HOST="0.0.0.0"
```

```shell
bash run_server.sh
```

## Run tests

```shell
bash run_tests.sh
```
