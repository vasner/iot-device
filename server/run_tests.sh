#!/bin/bash

# Runs iot-devise server tests

set -e

poetry run python -m pytest
