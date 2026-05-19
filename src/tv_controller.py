from src.remote_key import RemoteKey
from src.tuner import Tuner


class TVController:
    def __init__(self, tuner: Tuner):
        self._tuner = tuner
        self._processing_ch = ""

    def _set_tuner_ch(self):
        # 로그는 테스트의 결과가 절대 아닙니다. 로그가 있는 것을 테스트로 간주하지 마시기 바랍니다.
        print(f"현재 설정하는 채널 : {self._processing_ch}")
        # self._tuner.set_ch(self._processing_ch)

    def push_button(self, key: RemoteKey):
        if key == RemoteKey.KEY_1:
            self._processing_ch += str(key)
        elif key == RemoteKey.KEY_OK:
            self._set_tuner_ch()
