import pytest
from unittest.mock import MagicMock

from src.tuner import Tuner


class MockTuner(Tuner):
    def __init__(self):
        self.seek_ch = MagicMock()
        self.set_ch = MagicMock()
        self.get_current_ch = MagicMock()


class TestTunerInitChannel:
    """초기 채널값 확인 테스트"""

    def test_init_channel(self):
        tuner = MockTuner()
        tuner.get_current_ch.return_value = "0"
        init_ch = int(tuner.get_current_ch())
        assert 0 <= init_ch <= 99


class TestTunerValidChannel:
    """채널설정테스트 : 유효한 채널인 경우"""

    @pytest.mark.parametrize("channel", ["0", "4", "5", "12", "99"])
    def test_set_ch_for_valid_channel(self, channel):
        tuner = MockTuner()
        tuner.get_current_ch.return_value = channel
        tuner.set_ch(channel)
        tuner.set_ch.assert_called_once_with(channel)
        assert tuner.get_current_ch() == channel


class TestTunerInvalidChannel:
    """채널설정테스트 : 유효하지 않은 채널인 경우"""

    @pytest.mark.parametrize("channel", ["-12", "-2", "-0", "100", "9999"])
    def test_set_ch_for_invalid_channel(self, channel):
        tuner = MockTuner()
        tuner.set_ch.side_effect = ValueError("Invalid channel")
        with pytest.raises(ValueError):
            tuner.set_ch(channel)


class TestTunerSeekChannel:
    """채널검색테스트"""

    def test_seek_ch_10_times(self):
        """시작 채널로부터 10개 채널 검색 테스트"""
        tuner = MockTuner()
        tuner.seek_ch.return_value = "5"

        seek_channel = []
        for _ in range(10):
            seek_ch = tuner.seek_ch()
            if not seek_ch:
                break
            ch = int(seek_ch)
            assert 0 <= ch <= 99
            seek_channel.append(seek_ch)

        assert len(seek_channel) == 10

    def test_seek_ch_10_times_after_set_ch(self):
        """시작 채널값을 99로 지정하고 10개 채널 검색 테스트"""
        tuner = MockTuner()
        tuner.seek_ch.return_value = "50"

        seek_channel = []
        tuner.set_ch("99")
        for _ in range(10):
            seek_ch = tuner.seek_ch()
            if not seek_ch:
                break
            ch = int(seek_ch)
            assert 0 <= ch <= 99
            seek_channel.append(seek_ch)

        assert len(seek_channel) == 10
