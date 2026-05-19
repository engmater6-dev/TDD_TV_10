from abc import ABC, abstractmethod
from typing import List


class Tuner(ABC):
    @abstractmethod
    def seek_ch(self) -> List[str]:
        """현재 채널에서 숫자가 증가하는 방향으로 시청 가능한 채널을 검색하여,
        채널을 검색된 채널로 변경하고 검색된 채널값을 반환한다."""
        pass

    @abstractmethod
    def set_ch(self, ch: str) -> None:
        """지정된 채널값으로 현재 채널을 변경한다."""
        pass

    @abstractmethod
    def get_current_ch(self) -> str:
        """현재 설정된 채널 값을 반환한다."""
        pass


class MockTuner(Tuner):
    """
    외부 Tuner 모듈을 대체하는 Mock 클래스.
    실제 하드웨어 대신 테스트용으로 동작.
    """

    def __init__(self):
        self.current_channel: str = "0"
        self.channels: List[str] = [str(i) for i in range(100)]  # "0" ~ "99"

    def seek_ch(self) -> List[str]:
        """
        전체 채널 검색 결과 반환.
        실제 구현에서는 하드웨어 검색을 수행하지만,
        Mock에서는 단순히 0~99 문자열 리스트를 반환.
        """
        return self.channels

    def set_ch(self, ch: str) -> None:
        """
        현재 채널 설정.
        범위(0~99) 밖의 값은 무효 처리.
        """
        if ch in self.channels:
            self.current_channel = ch

    def get_current_ch(self) -> str:
        """
        현재 채널 반환.
        """
        return self.current_channel
