#include "TVController.h"
#include "ITuner.h"
#include "remoteKey.h"
#include <gtest/gtest.h>

// FakeTuner: 채널을 메모리에 저장
class FakeTuner : public ITuner {
private:
  std::string currentCH;

public:
  void setCH(std::string ch) override { currentCH = ch; }
  std::string seekCH() override { return ""; }
  std::string getCurrentCH() override { return currentCH; }
};

// 공통 픽스처
class ControllerTest : public ::testing::Test {
protected:
  std::unique_ptr<FakeTuner> tuner;
  std::unique_ptr<TVController> ctrl;

  void SetUp() override {
    tuner = std::make_unique<FakeTuner>();
    ctrl = std::make_unique<TVController>(*tuner);
  }
};

// S1-1: 한 자리 입력 + 확인
TEST_F(ControllerTest, OneDigitWithConfirm) {
  ctrl->pushButton(remoteKey::KEY_1);
  ctrl->pushButton(remoteKey::KEY_OK);
  EXPECT_EQ("1", tuner->getCurrentCH());
}

// S1-2: 두 자리 자동 변경
TEST_F(ControllerTest, TwoDigitAutoChange) {
  ctrl->pushButton(remoteKey::KEY_1);
  ctrl->pushButton(remoteKey::KEY_2);
  EXPECT_EQ("12", tuner->getCurrentCH());
}

// S1-3: 세 자리 입력 (123 → 12 → 3)
TEST_F(ControllerTest, ThreeDigitInput) {
  ctrl->pushButton(remoteKey::KEY_1);
  ctrl->pushButton(remoteKey::KEY_2);
  EXPECT_EQ("12", tuner->getCurrentCH());
  ctrl->pushButton(remoteKey::KEY_3);
  ctrl->pushButton(remoteKey::KEY_OK); // OK 눌러야 확정
  EXPECT_EQ("3", tuner->getCurrentCH());
}

// S1-4: 0으로 시작하는 두 자리 (01 → 1)
TEST_F(ControllerTest, LeadingZeroTwoDigit) {
  ctrl->pushButton(remoteKey::KEY_0);
  ctrl->pushButton(remoteKey::KEY_1);
  EXPECT_EQ("1", tuner->getCurrentCH());
}

// S1-5: 네 자리 연속 입력 (1234 → 12 → 34)
TEST_F(ControllerTest, FourDigitInput) {
  ctrl->pushButton(remoteKey::KEY_1);
  ctrl->pushButton(remoteKey::KEY_2);
  EXPECT_EQ("12", tuner->getCurrentCH()); // 첫 두 자리 확정

  ctrl->pushButton(remoteKey::KEY_3);
  ctrl->pushButton(remoteKey::KEY_4);
  EXPECT_EQ("34", tuner->getCurrentCH()); // 다음 두 자리 확정
}

// S1-6: 경계값 테스트 - 채널 99
TEST_F(ControllerTest, BoundaryChannel99) {
  ctrl->pushButton(remoteKey::KEY_9);
  ctrl->pushButton(remoteKey::KEY_9); // 두 자리 완성 → 자동
  EXPECT_EQ("99", tuner->getCurrentCH());
}

// S1-7: 경계값 테스트 - 채널 00 (leading zero → 0)
TEST_F(ControllerTest, BoundaryChannel00) {
  ctrl->pushButton(remoteKey::KEY_0);
  ctrl->pushButton(remoteKey::KEY_0); // "00" → 채널 0
  EXPECT_EQ("0", tuner->getCurrentCH());
}

// S1-8: 경계값 테스트 - 채널 09 (leading zero → 9)
TEST_F(ControllerTest, BoundaryChannel09) {
  ctrl->pushButton(remoteKey::KEY_0);
  ctrl->pushButton(remoteKey::KEY_9); // "09" → 채널 9
  EXPECT_EQ("9", tuner->getCurrentCH());
}
