from abc import ABC, abstractmethod


class Tuner(ABC):
    @abstractmethod
    def seek_ch(self) -> str:
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
