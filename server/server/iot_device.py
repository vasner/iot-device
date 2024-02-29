import serial
import datetime
import threading
import logging


class IotDevice:
    _update_rate_s: float
    _device: serial.Serial
    _sample_json: str = "{}"
    _timer: threading.Timer = None
    _version: str = ""

    def __init__(self, port: str = "/dev/ttyACM1", update_rate_s: float = 0.1) -> None:
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
            self._timer = threading.Timer(self._update_rate_s, self._read_sample)

    def __del__(self):
        self._device.close()
        if self._timer is not None:
            self._timer.cancel()

    @property
    def version(self) -> str:
        return self._version

    def get_sample_json(self) -> str:
        return self._sample_json

    def _read_sample(self):
        print(datetime.datetime.now())
        # self._device.write("sample\r\n".encode())
        # self._device.readline().decode()

    def _read_version(self) -> str:
        self._device.write("version\r\n".encode())
        return self._device.readline().decode().replace("\n", "")
