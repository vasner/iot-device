#!/bin/bash

# Runs iot-device server tests

set -e

poetry run python -m pytest
