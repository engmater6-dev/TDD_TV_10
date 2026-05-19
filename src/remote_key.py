from enum import Enum


class RemoteKey(Enum):
    """리모컨 키 정의"""

    # 숫자 키
    KEY_0 = "0"
    KEY_1 = "1"
    KEY_2 = "2"
    KEY_3 = "3"
    KEY_4 = "4"
    KEY_5 = "5"
    KEY_6 = "6"
    KEY_7 = "7"
    KEY_8 = "8"
    KEY_9 = "9"

    # 기능 키
    KEY_OK = "OK"
    KEY_UP = "UP"
    KEY_DOWN = "DOWN"
    KEY_SEARCH = "SEARCH"
    KEY_ADD_FAVORITE = "ADD_FAVORITE"
    KEY_NEXT_FAVORITE = "NEXT_FAVORITE"

    def __str__(self) -> str:
        return self.value
