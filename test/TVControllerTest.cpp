#include "TVController.h"
#include "ITuner.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

class MockTunerForController : public ITuner {
public:
  MOCK_METHOD(std::string, seekCH, (), (override));
  MOCK_METHOD(void, setCH, (std::string ch), (override));
  MOCK_METHOD(std::string, getCurrentCH, (), (override));
};

// 1자리 숫자 입력 후 OK 버튼을 눌렀을 때 setCH가 호출되는지 테스트
TEST(TVControllerTest, OneInputAndOkSetCh) {
  MockTunerForController mockTuner;
  TVController controller(&mockTuner);

  EXPECT_CALL(mockTuner, setCH("1")).Times(1);

  controller.pushButton(remoteKey::KEY_1);
  controller.pushButton(remoteKey::KEY_OK);
}

// 2자리 숫자 입력 후 OK 버튼을 누르지 않아도 setCH가 호출되는지 테스트
TEST(TVControllerTest, TwoInputAutoSetCh) {
  MockTunerForController mockTuner;
  TVController controller(&mockTuner);

  EXPECT_CALL(mockTuner, setCH("12")).Times(1);

  controller.pushButton(remoteKey::KEY_1);
  controller.pushButton(remoteKey::KEY_2);
}
