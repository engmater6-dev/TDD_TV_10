import pytest
from unittest.mock import MagicMock

from src.tuner import Tuner
from src.tv_controller import TVController


class MockTuner(Tuner):
    def __init__(self):
        self.seek_ch = MagicMock()
        self.set_ch = MagicMock()
        self.get_current_ch = MagicMock()


class TestTVController:
    def test_framework(self):
        # fail()
        pass
