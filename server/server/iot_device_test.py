from .iot_device import IotDevice


def test_iot_device():
    dev = IotDevice()
    assert dev.version == "0.3.0"
