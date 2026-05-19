# Test Plan: TV Controller Project (Python 3, pytest, pytest-cov)

## 1. 단위 테스트 범위 및 우선순위
- **숫자 입력 및 채널 변경 (High Priority)**
  - 단일 숫자 입력 후 OK
  - 연속 숫자 입력 후 OK
  - `0` + `7` → 7번 채널
- **업/다운 버튼 동작 (High Priority)**
  - 검색 결과 없음 → 단순 ±1 이동
  - 검색 결과 있음 → 목록 내 이동
- **선호 채널 관리 (Medium Priority)**
  - 추가/삭제 동작
  - 다음 선호 채널 이동 (순환 포함)
- **검색 버튼 동작 (Medium Priority)**
  - 전체 채널 검색(Mock Tuner 활용)
- **예외/경계 조건 (High Priority)**
  - 범위 초과 입력
  - 빈 선호 목록에서 `NEXT_FAVORITE`
  - 검색 결과 없음 상태에서 업/다운

---

## 2. 경계값 케이스 목록 (@pytest.mark.parametrize 활용)
```python
@pytest.mark.parametrize("input_seq,expected", [
    ([RemoteKey.KEY_0, RemoteKey.KEY_7, RemoteKey.KEY_OK], "7"),   # 0+7 → 7
    ([RemoteKey.KEY_9, RemoteKey.KEY_9, RemoteKey.KEY_OK], "99"), # 최대 채널
    ([RemoteKey.KEY_1, RemoteKey.KEY_OK], "1"),                   # 최소 유효 입력
])
def test_channel_boundary_cases(controller, input_seq, expected):
    for key in input_seq:
        result = controller.push_button(key)
    assert result == expected
