import os
import pytest

from .iot_device import IotDevice


def test_iot_device():
    device_port = os.getenv("IOT_DEVICE_PORT") or "/dev/ttyACM1"
    try:
        device = IotDevice(device_port)
    except Exception:
        pytest.skip("Device is not connected or failed to initialize")
    else:
        version = device.version
        device.close()
        assert version == "0.4.0"
