# Code Quality Report: test_tv_controller.py

| 문제점 | 위반 원칙/스멜 | 영향 | 개선 방향 | 우선순위 |
|--------|----------------|------|-----------|----------|
| 단일 클래스에 모든 시나리오 테스트 포함 | SRP 위반 | 테스트 클래스가 너무 많은 책임을 가짐 → 유지보수 어려움 | 시나리오별로 테스트 클래스를 분리 (예: NumberInputTests, FavoriteChannelTests 등) | 1 |
| `assert result in ["45", "46"]` 같은 모호한 조건 | OCP 위반, Magic Number | 테스트 결과가 불명확 → 리팩토링 시 오작동 가능 | 명확한 기대값 정의, 버퍼 처리 로직을 전략 패턴으로 분리하여 확장 가능하게 설계 | 2 |
| 긴 메서드 (예: test_number_input_and_confirm) | Long Method | 가독성 저하, 중복 로직 발생 | 공통 로직을 헬퍼 메서드로 추출 (`_press_sequence`) | 3 |
| 문자열 하드코딩 ("1", "12", "99") | Magic Number/Hardcoded String | 변경 시 다수 테스트 수정 필요 | 상수/픽스처 활용, Enum 기반 입력값 사용 | 4 |
| 중복 코드 (숫자 입력 후 OK 반복) | Duplicated Code | 테스트 유지보수 비용 증가 | pytest fixture + 파라미터화 활용 | 3 |
| 조건 복잡도 (favorites, search_results 테스트) | 조건문 복잡도 | 테스트 의도 파악 어려움 | dataclass 기반 상태 관리, 헬퍼 함수로 조건 단순화 | 5 |
| 타입 힌트 부재 | Python 특화 문제 | IDE 지원 및 정적 분석 제한 | 함수 시그니처에 타입 힌트 추가 (`-> str | None`) | 2 |
| PEP 8 미준수 (메서드 길이, assert 스타일) | Pythonic 개선 필요 | 코드 일관성 부족 | pytest의 `parametrize`, 헬퍼 함수, 전략 패턴 적용 | 4 |

---

## 🔑 리팩토링 우선순위
1. **SRP 위반 해결** → 테스트 클래스 분리 (가장 큰 유지보수 문제)  
2. **명확한 기대값 정의 및 타입 힌트 추가** → 테스트 신뢰성 확보  
3. **중복 코드 제거 및 긴 메서드 분리** → 가독성 개선  
4. **문자열 하드코딩 제거 및 PEP 8 준수** → Pythonic 코드로 개선  
5. **조건 복잡도 단순화** → 장기적으로 테스트 유지보수성 강화  
