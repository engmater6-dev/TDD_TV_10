from src.remote_key import RemoteKey
from src.tuner import Tuner


class TVController:
    def __init__(self, tuner: Tuner):
        self._tuner = tuner
        self._processing_ch: str = ""   # 숫자 입력 버퍼
        self._favorites: list[str] = [] # 선호 채널 목록
        self._search_results: list[str] = [] # 검색 결과 채널 목록

    def _set_tuner_ch(self):
        """버퍼에 있는 채널을 실제 Tuner에 반영"""
        if self._processing_ch and self._processing_ch.isdigit():
            if 0 <= int(self._processing_ch) <= 99:
                self._tuner.set_ch(self._processing_ch)
        self._processing_ch = ""  # 버퍼 초기화

    def push_button(self, key: RemoteKey) -> str | None:
        """리모컨 키 입력 처리"""
        if key.value.isdigit():  # 숫자 키
            self._processing_ch += key.value
            return None

        if key == RemoteKey.KEY_OK:
            self._set_tuner_ch()
            return self._tuner.get_current_ch()

        if key == RemoteKey.KEY_UP:
            return self._handle_up()

        if key == RemoteKey.KEY_DOWN:
            return self._handle_down()

        if key == RemoteKey.KEY_SEARCH:
            return self._handle_search()

        if key == RemoteKey.KEY_ADD_FAVORITE:
            return self._handle_favorite()

        if key == RemoteKey.KEY_NEXT_FAVORITE:
            return self._handle_next_favorite()

        # 다른 버튼 입력 시 숫자 버퍼 무효화
        self._processing_ch = ""
        return None

    def _handle_up(self) -> str:
        current = int(self._tuner.get_current_ch())
        if self._search_results:
            return self._move_in_list(current, direction=1)
        next_ch = (current + 1) % 100
        self._tuner.set_ch(str(next_ch))
        return str(next_ch)

    def _handle_down(self) -> str:
        current = int(self._tuner.get_current_ch())
        if self._search_results:
            return self._move_in_list(current, direction=-1)
        next_ch = (current - 1) % 100
        self._tuner.set_ch(str(next_ch))
        return str(next_ch)

    def _handle_search(self) -> str:
        self._search_results = self._tuner.seek_ch()
        return self._tuner.get_current_ch()

    def _handle_favorite(self) -> str:
        current = self._tuner.get_current_ch()
        if current in self._favorites:
            self._favorites.remove(current)
        else:
            self._favorites.append(current)
        return current

    def _handle_next_favorite(self) -> str | None:
        if not self._favorites:
            return None
        current = int(self._tuner.get_current_ch())
        sorted_favs = sorted(int(ch) for ch in self._favorites)
        larger = [ch for ch in sorted_favs if ch > current]
        next_ch = larger[0] if larger else sorted_favs[0]
        self._tuner.set_ch(str(next_ch))
        return str(next_ch)

    def _move_in_list(self, current: int, direction: int) -> str:
        if str(current) not in self._search_results:
            # 가장 가까운 값으로 이동
            closest = min(self._search_results, key=lambda x: abs(int(x) - current))
            self._tuner.set_ch(closest)
            return closest
        idx = self._search_results.index(str(current))
        next_idx = (idx + direction) % len(self._search_results)
        next_ch = self._search_results[next_idx]
        self._tuner.set_ch(next_ch)
        return next_ch
