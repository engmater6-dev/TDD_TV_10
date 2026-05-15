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

TEST(TVControllerTest, OneInputAndOkSetCh) {
  MockTunerForController mockTuner;
  TVController controller(&mockTuner);

  EXPECT_CALL(mockTuner, setCH("1")).Times(1);

  controller.pushButton(remoteKey::KEY_1);
  controller.pushButton(remoteKey::KEY_OK);
}
