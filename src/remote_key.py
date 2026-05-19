from enum import Enum


class RemoteKey(Enum):
    KEY_1 = "1"
    KEY_OK = "OK"

    def __str__(self):
        return self.value
