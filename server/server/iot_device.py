import serial
import threading
import logging


class IotDevice:
    _update_rate_s: float
    _sample_json: str = "{}"
    _device: serial.Serial = None
    _timer: threading.Timer = None
    _version: str = ""

    def __init__(self, port: str = "/dev/ttyACM1", update_rate_s: float = 1.0) -> None:
        self._update_rate_s = update_rate_s
        try:
            self._device = serial.Serial(
                port=port,
                baudrate=115200,
                parity=serial.PARITY_NONE,
                stopbits=serial.STOPBITS_ONE,
                bytesize=serial.EIGHTBITS,
            )
        except Exception as e:
            logging.error(f"Unable to open device on port: `{port}`")
            logging.error(e)
            raise ConnectionError from e
        else:
            self._version = self._read_version()
            self._continuous_poll_sample()

    def __del__(self):
        self.close()

    def close(self):
        if self._timer is not None:
            self._timer.cancel()
        if self._device is not None:
            self._device.close()

    @property
    def version(self) -> str:
        return self._version

    def get_sample_json(self) -> str:
        return self._sample_json

    def _continuous_poll_sample(self):
        self._device.write("sample\r\n".encode())
        self._sample_json = IotDevice._clear_device_response(self._device.readline().decode())
        self._timer = threading.Timer(self._update_rate_s, self._continuous_poll_sample)
        self._timer.start()

    def _read_version(self) -> str:
        self._device.write("version\r\n".encode())
        return IotDevice._clear_device_response(self._device.readline().decode())

    @staticmethod
    def _clear_device_response(response: str):
        return response.replace("\n", "").replace(">>> ", "")
