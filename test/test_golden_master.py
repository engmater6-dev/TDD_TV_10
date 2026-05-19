import io
import os
import subprocess
import pytest

GOLDEN_FILE = os.path.join("tests", "golden_master_expected.txt")
CURRENT_FILE = os.path.join("tests", "golden_master_actual.txt")


@pytest.mark.golden_master
def test_tv_controller_output_matches_golden_master():
    # Given: TVController 실행 (stdout 캡처)
    result = subprocess.run(
        ["python", "src/tv_controller.py"], capture_output=True, text=True
    )
    actual_output = result.stdout

    # When: Golden Master 파일 존재 여부 확인
    if not os.path.exists(GOLDEN_FILE):
        # 기준 파일 없으면 생성
        with open(GOLDEN_FILE, "w", encoding="utf-8") as f:
            f.write(actual_output)
        pytest.skip("Golden Master 파일 생성됨. 다음 실행부터 비교합니다.")

    # Then: Golden Master와 비교
    with open(GOLDEN_FILE, "r", encoding="utf-8") as f:
        expected_output = f.read()

    # 현재 출력 저장 (디버깅용)
    with open(CURRENT_FILE, "w", encoding="utf-8") as f:
        f.write(actual_output)

    assert actual_output == expected_output, "출력이 Golden Master와 다릅니다."
