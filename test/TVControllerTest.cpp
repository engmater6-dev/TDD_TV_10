#include "TVController.h"
#include "ITuner.h"
#include "remoteKey.h"
#include <gtest/gtest.h>
#include <memory>
#include <string>
#include <vector>

// FakeTuner: 채널을 메모리에 저장, 범위 검증 포함
class FakeTuner : public ITuner {
private:
  int current_ = 0;

public:
  void setCH(std::string ch) override {
    int v = std::stoi(ch);
    if (v < 0 || v > 99)
      throw std::invalid_argument("채널 범위 초과: " + ch);
    current_ = v;
  }
  std::string seekCH() override {
    current_ = (current_ + 1) % 100;
    return std::to_string(current_);
  }
  std::string getCurrentCH() override { return std::to_string(current_); }
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

// S1-3: 세 자리 입력 (456 → 45 → 이후 6 처리)
TEST_F(ControllerTest, ThreeDigitInputBehavior) {
  // 4,5 입력 → 자동으로 45번 채널 변경
  ctrl->pushButton(remoteKey::KEY_4);
  ctrl->pushButton(remoteKey::KEY_5);
  EXPECT_EQ("45", tuner->getCurrentCH());

  // 6 입력 → 아직 확정되지 않고 버퍼에 남음
  ctrl->pushButton(remoteKey::KEY_6);
  // 현재 채널은 여전히 45
  EXPECT_EQ("45", tuner->getCurrentCH());

  // 확인 버튼 누르면 6번 채널로 이동
  ctrl->pushButton(remoteKey::KEY_OK);
  EXPECT_EQ("6", tuner->getCurrentCH());

  // 다시 7 입력 후 기타 버튼 누르면 무효화
  ctrl->pushButton(remoteKey::KEY_7);
  ctrl->pushButton(remoteKey::KEY_OTHER);
  ctrl->pushButton(remoteKey::KEY_OK);
  // 채널은 그대로 6 유지 (7은 무효화됨)
  EXPECT_EQ("6", tuner->getCurrentCH());
}

// S1-4: 0으로 시작하는 두 자리 (01 → 1)
TEST_F(ControllerTest, LeadingZeroTwoDigit) {
  ctrl->pushButton(remoteKey::KEY_0);
  ctrl->pushButton(remoteKey::KEY_1);
  EXPECT_EQ("1", tuner->getCurrentCH());
}

// S1-5: 네 자리 입력 (1234 → 12 → 34)
TEST_F(ControllerTest, FourDigitInput) {
  ctrl->pushButton(remoteKey::KEY_1);
  ctrl->pushButton(remoteKey::KEY_2);
  EXPECT_EQ("12", tuner->getCurrentCH());
  ctrl->pushButton(remoteKey::KEY_3);
  ctrl->pushButton(remoteKey::KEY_4);
  EXPECT_EQ("34", tuner->getCurrentCH());
}

// S1-6: 경계값 테스트 - 채널 99
TEST_F(ControllerTest, BoundaryChannel99) {
  ctrl->pushButton(remoteKey::KEY_9);
  ctrl->pushButton(remoteKey::KEY_9);
  EXPECT_EQ("99", tuner->getCurrentCH());
}

// S1-7: 경계값 테스트 - 채널 00
TEST_F(ControllerTest, BoundaryChannel00) {
  ctrl->pushButton(remoteKey::KEY_0);
  ctrl->pushButton(remoteKey::KEY_0);
  EXPECT_EQ("0", tuner->getCurrentCH());
}

// S1-8: 경계값 테스트 - 채널 09
TEST_F(ControllerTest, BoundaryChannel09) {
  ctrl->pushButton(remoteKey::KEY_0);
  ctrl->pushButton(remoteKey::KEY_9);
  EXPECT_EQ("9", tuner->getCurrentCH());
}

// S1-10: 기타 버튼 입력 → 버퍼 무효화
TEST_F(ControllerTest, OtherButtonClearsBuffer) {
  ctrl->pushButton(remoteKey::KEY_1);
  ctrl->pushButton(remoteKey::KEY_OTHER);
  ctrl->pushButton(remoteKey::KEY_OK);
  EXPECT_EQ("0", tuner->getCurrentCH()); // 변경 없음
}

// S1-9: seekCH 호출 테스트
TEST_F(ControllerTest, SeekCHCall) {
  ctrl->pushButton(remoteKey::KEY_1);
  ctrl->pushButton(remoteKey::KEY_OK);
  EXPECT_EQ("2", tuner->seekCH()); // 다음 채널
}

// S2-1: 선호 채널 추가/삭제
TEST_F(ControllerTest, ToggleFavoriteChannel) {
  ctrl->pushButton(remoteKey::KEY_5);
  ctrl->pushButton(remoteKey::KEY_OK);
  ctrl->pushButton(remoteKey::KEY_FAVORITE);
  EXPECT_EQ(1u, ctrl->getController().getFavoriteChannels().size());
  ctrl->pushButton(remoteKey::KEY_FAVORITE);
  EXPECT_TRUE(ctrl->getController().getFavoriteChannels().empty());
}

// S2-2: 여러 채널 추가 후 정렬 검증
TEST_F(ControllerTest, FavoritesAreSorted) {
  ctrl->pushButton(remoteKey::KEY_9);
  ctrl->pushButton(remoteKey::KEY_OK);
  ctrl->pushButton(remoteKey::KEY_FAVORITE);
  ctrl->pushButton(remoteKey::KEY_1);
  ctrl->pushButton(remoteKey::KEY_OK);
  ctrl->pushButton(remoteKey::KEY_FAVORITE);
  auto favs = ctrl->getController().getFavoriteChannels();
  EXPECT_EQ((std::vector<int>{1, 9}), favs);
}

// S3-1: 선호 채널 이동
TEST_F(ControllerTest, MoveToNextFavorite) {
  ctrl->pushButton(remoteKey::KEY_1);
  ctrl->pushButton(remoteKey::KEY_OK);
  ctrl->pushButton(remoteKey::KEY_FAVORITE);
  ctrl->pushButton(remoteKey::KEY_9);
  ctrl->pushButton(remoteKey::KEY_OK);
  ctrl->pushButton(remoteKey::KEY_FAVORITE);
  ctrl->pushButton(remoteKey::KEY_1);
  ctrl->pushButton(remoteKey::KEY_OK);
  ctrl->pushButton(remoteKey::KEY_NEXT_FAVORITE);
  EXPECT_EQ("9", tuner->getCurrentCH());
}

// S3-2: 현재 채널이 목록 외 값일 때
TEST_F(ControllerTest, NextFavoriteFromNonFavoriteChannel) {
  ctrl->pushButton(remoteKey::KEY_2);
  ctrl->pushButton(remoteKey::KEY_OK);
  ctrl->pushButton(remoteKey::KEY_FAVORITE);
  ctrl->pushButton(remoteKey::KEY_5);
  ctrl->pushButton(remoteKey::KEY_OK);
  ctrl->pushButton(remoteKey::KEY_NEXT_FAVORITE);
  EXPECT_EQ("2", tuner->getCurrentCH());
}

// S3-3: 목록이 비어있을 때
TEST_F(ControllerTest, NextFavoriteWithEmptyListDoesNothing) {
  ctrl->pushButton(remoteKey::KEY_1);
  ctrl->pushButton(remoteKey::KEY_OK);
  ctrl->pushButton(remoteKey::KEY_NEXT_FAVORITE);
  EXPECT_EQ("1", tuner->getCurrentCH());
}

// S5: 업/다운 동작 (전체 채널 기준)
TEST_F(ControllerTest, ChannelUpDownWithoutFavorites) {
  ctrl->pushButton(remoteKey::KEY_0);
  ctrl->pushButton(remoteKey::KEY_OK);
  ctrl->pushButton(remoteKey::KEY_UP);
  EXPECT_EQ("1", tuner->getCurrentCH());
  ctrl->pushButton(remoteKey::KEY_DOWN);
  EXPECT_EQ("0", tuner->getCurrentCH());
}

// S6: 업/다운 동작 (즐겨찾기 기준)
TEST_F(ControllerTest, ChannelUpDownWithFavorites) {
  ctrl->pushButton(remoteKey::KEY_2);
  ctrl->pushButton(remoteKey::KEY_OK);
  ctrl->pushButton(remoteKey::KEY_FAVORITE);
  ctrl->pushButton(remoteKey::KEY_5);
  ctrl->pushButton(remoteKey::KEY_OK);
  ctrl->pushButton(remoteKey::KEY_FAVORITE);
  ctrl->pushButton(remoteKey::KEY_2);
  ctrl->pushButton(remoteKey::KEY_OK);
  ctrl->pushButton(remoteKey::KEY_UP_FAVORITE);
  EXPECT_EQ("5", tuner->getCurrentCH());
  ctrl->pushButton(remoteKey::KEY_DOWN_FAVORITE);
  EXPECT_EQ("2", tuner->getCurrentCH());
}

// S4-2: Tuner setCH 유효값 → 정상 동작
TEST_F(ControllerTest, TunerSetCHValid) {
  tuner->setCH("10");
  EXPECT_EQ("10", tuner->getCurrentCH());
}

// S4-3: Tuner setCH 유효하지 않은 값 → 예외
TEST_F(ControllerTest, TunerSetCHInvalid) {
  EXPECT_THROW(tuner->setCH("999"), std::invalid_argument);
}

// S4-4: seekCH wrap-around 검증
TEST_F(ControllerTest, TunerSeekCHWrapAround) {
  tuner->setCH("98");
  EXPECT_EQ("99", tuner->seekCH()); // 98 → 99
  EXPECT_EQ("0", tuner->seekCH());  // wrap-around → 0
  EXPECT_EQ("1", tuner->seekCH());  // 0 → 1
}

// S4-5: getCurrentCH 현재값 반환 검증
TEST_F(ControllerTest, TunerGetCurrentCH) {
  tuner->setCH("42");
  EXPECT_EQ("42", tuner->getCurrentCH());
}
