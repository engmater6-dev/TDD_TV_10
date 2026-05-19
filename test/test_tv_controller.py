import pytest
from src.remote_key import RemoteKey
from src.tuner import MockTuner
from src.tv_controller import TVController


@pytest.fixture
def controller():
    tuner = MockTuner()
    return TVController(tuner)


# 숫자 입력 및 채널 변경
def test_number_input_confirm_changes_channel(controller):
    # Given: 숫자 입력 1
    controller.push_button(RemoteKey.KEY_1)
    # When: OK 버튼 누름
    result = controller.push_button(RemoteKey.KEY_OK)
    # Then: 채널은 "1"로 변경
    assert result == "1"


def test_number_input_two_digits_channel_change(controller):
    # Given: 숫자 입력 1, 2
    controller.push_button(RemoteKey.KEY_1)
    controller.push_button(RemoteKey.KEY_2)
    # When: OK 버튼 누름
    result = controller.push_button(RemoteKey.KEY_OK)
    # Then: 채널은 "12"로 변경
    assert result == "12"


def test_number_input_zero_seven_channel_change(controller):
    # Given: 숫자 입력 0, 7
    controller.push_button(RemoteKey.KEY_0)
    controller.push_button(RemoteKey.KEY_7)
    # When: OK 버튼 누름
    result = controller.push_button(RemoteKey.KEY_OK)
    # Then: 채널은 "7"로 변경
    assert result == "7"


# 업/다운 버튼 동작
def test_up_button_wraps_from_99_to_0(controller):
    # Given: 현재 채널 99
    controller._tuner.set_ch("99")
    # When: UP 버튼 누름
    result = controller.push_button(RemoteKey.KEY_UP)
    # Then: 채널은 "0"으로 순환
    assert result == "0"


def test_down_button_wraps_from_0_to_99(controller):
    # Given: 현재 채널 0
    controller._tuner.set_ch("0")
    # When: DOWN 버튼 누름
    result = controller.push_button(RemoteKey.KEY_DOWN)
    # Then: 채널은 "99"로 순환
    assert result == "99"


# 선호 채널
def test_add_favorite_channel(controller):
    # Given: 현재 채널 12
    controller._tuner.set_ch("12")
    # When: ADD_FAVORITE 버튼 누름
    result = controller.push_button(RemoteKey.KEY_ADD_FAVORITE)
    # Then: 채널 12가 선호 목록에 추가됨
    assert result == "12"
    assert "12" in controller._favorites


def test_remove_favorite_channel(controller):
    # Given: 현재 채널 12, 이미 선호 목록에 있음
    controller._tuner.set_ch("12")
    controller.push_button(RemoteKey.KEY_ADD_FAVORITE)
    # When: 다시 ADD_FAVORITE 버튼 누름
    result = controller.push_button(RemoteKey.KEY_ADD_FAVORITE)
    # Then: 채널 12가 선호 목록에서 제거됨
    assert result == "12"
    assert "12" not in controller._favorites


def test_next_favorite_channel_wraps(controller):
    # Given: 선호 목록 [1, 4, 12, 56], 현재 채널 56
    for ch in ["1", "4", "12", "56"]:
        controller._tuner.set_ch(ch)
        controller.push_button(RemoteKey.KEY_ADD_FAVORITE)
    controller._tuner.set_ch("56")
    # When: NEXT_FAVORITE 버튼 누름
    result = controller.push_button(RemoteKey.KEY_NEXT_FAVORITE)
    # Then: 채널은 "1"로 순환
    assert result == "1"


# 검색
def test_search_button_stores_channels(controller):
    # Given: MockTuner
    # When: SEARCH 버튼 누름
    result = controller.push_button(RemoteKey.KEY_SEARCH)
    # Then: search_results에 0~99 채널 저장
    assert controller._search_results == [str(i) for i in range(100)]
    assert result == controller._tuner.get_current_ch()


def test_up_down_with_search_results(controller):
    # Given: 검색 결과 [4, 6, 14], 현재 채널 6
    controller._search_results = ["4", "6", "14"]
    controller._tuner.set_ch("6")
    # When: UP 버튼 누름
    result = controller.push_button(RemoteKey.KEY_UP)
    # Then: 채널은 "14"
    assert result == "14"

    # Given: 현재 채널 15 (검색 결과에 없음)
    controller._tuner.set_ch("15")
    # When: DOWN 버튼 누름
    result = controller.push_button(RemoteKey.KEY_DOWN)
    # Then: 가장 가까운 채널 "14"로 이동
    assert result == "14"


# 예외/경계 조건
def test_invalid_channel_input_is_ignored(controller):
    # Given: 숫자 입력 1, 0, 0 (100)
    controller.push_button(RemoteKey.KEY_1)
    controller.push_button(RemoteKey.KEY_0)
    controller.push_button(RemoteKey.KEY_0)
    # When: OK 버튼 누름
    result = controller.push_button(RemoteKey.KEY_OK)
    # Then: 무효 처리 → None 반환
    assert result is None


def test_next_favorite_with_empty_list_returns_none(controller):
    # Given: 선호 목록이 비어있음
    controller._tuner.set_ch("5")
    # When: NEXT_FAVORITE 버튼 누름
    result = controller.push_button(RemoteKey.KEY_NEXT_FAVORITE)
    # Then: None 반환
    assert result is None


def test_up_down_without_search_results_moves_by_one(controller):
    # Given: 현재 채널 6, 검색 결과 없음
    controller._tuner.set_ch("6")
    # When: UP 버튼 누름
    result = controller.push_button(RemoteKey.KEY_UP)
    # Then: 채널은 "7"
    assert result == "7"

    # Given: 현재 채널 0
    controller._tuner.set_ch("0")
    # When: DOWN 버튼 누름
    result = controller.push_button(RemoteKey.KEY_DOWN)
    # Then: 채널은 "99"
    assert result == "99"
