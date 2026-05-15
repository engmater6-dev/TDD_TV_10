#define APPROVALS_GOOGLETEST
#include "ITuner.h"
#include "TVController.h"
#include "remoteKey.h"
#include <ApprovalTests.hpp>
#include <filesystem>
#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

// FakeTuner: 채널을 메모리에 저장, 범위 검증 포함
class FakeTuner : public ITuner {
private:
  int current_ = 0;

public:
  void setCH(std::string ch) override {
    try {
      int v = std::stoi(ch);
      if (v < 0 || v > 99)
        throw std::invalid_argument("채널 범위 초과: " + ch);
      current_ = v;
    } catch (const std::exception &ex) {
      // 예외 발생 시에도 verify까지 도달할 수 있도록 메시지 출력
      std::cout << "Exception in setCH: " << ex.what() << std::endl;
    }
  }
  std::string seekCH() override {
    current_ = (current_ + 1) % 100;
    return std::to_string(current_);
  }
  std::string getCurrentCH() override { return std::to_string(current_); }
};

// 공통 픽스처
class ControllerApprovalTest : public ::testing::Test {
protected:
  std::unique_ptr<FakeTuner> tuner;
  std::unique_ptr<TVController> ctrl;

  static std::unique_ptr<ApprovalTests::SubdirectoryDisposer> disposer;

  static void SetUpTestSuite() {
    // 결과 파일을 test 폴더에 강제 생성
    auto testDir = std::filesystem::absolute("test");
    std::filesystem::create_directories(testDir);
    disposer = std::make_unique<ApprovalTests::SubdirectoryDisposer>(
        ApprovalTests::Approvals::useApprovalsSubdirectory(testDir.string()));
  }

  void SetUp() override {
    tuner = std::make_unique<FakeTuner>();
    ctrl = std::make_unique<TVController>(*tuner);
  }
};

std::unique_ptr<ApprovalTests::SubdirectoryDisposer>
    ControllerApprovalTest::disposer;

TEST_F(ControllerApprovalTest, AllScenarios) {
  std::ostringstream out;

  // S1: 숫자 입력
  ctrl->pushButton(remoteKey::KEY_1);
  ctrl->pushButton(remoteKey::KEY_OK);
  out << "OneDigitWithConfirm=" << tuner->getCurrentCH() << "\n";

  ctrl->pushButton(remoteKey::KEY_1);
  ctrl->pushButton(remoteKey::KEY_2);
  out << "TwoDigitAutoChange=" << tuner->getCurrentCH() << "\n";

  ctrl->pushButton(remoteKey::KEY_4);
  ctrl->pushButton(remoteKey::KEY_5);
  out << "ThreeDigitFirstTwo=" << tuner->getCurrentCH() << "\n";
  ctrl->pushButton(remoteKey::KEY_6);
  out << "ThreeDigitBufferStill=" << tuner->getCurrentCH() << "\n";
  ctrl->pushButton(remoteKey::KEY_OK);
  out << "ThreeDigitFinalConfirm=" << tuner->getCurrentCH() << "\n";

  // S2: 즐겨찾기
  ctrl->pushButton(remoteKey::KEY_5);
  ctrl->pushButton(remoteKey::KEY_OK);
  ctrl->pushButton(remoteKey::KEY_FAVORITE);
  out << "AddFavorite=" << tuner->getCurrentCH() << " "
      << ctrl->getController().getFavoriteChannels().size() << "\n";
  ctrl->pushButton(remoteKey::KEY_FAVORITE);
  out << "RemoveFavorite=" << ctrl->getController().getFavoriteChannels().size()
      << "\n";

  // S3: 다음 즐겨찾기
  ctrl->pushButton(remoteKey::KEY_2);
  ctrl->pushButton(remoteKey::KEY_OK);
  ctrl->pushButton(remoteKey::KEY_FAVORITE);
  ctrl->pushButton(remoteKey::KEY_5);
  ctrl->pushButton(remoteKey::KEY_OK);
  ctrl->pushButton(remoteKey::KEY_NEXT_FAVORITE);
  out << "NextFavoriteFromNonFavorite=" << tuner->getCurrentCH() << "\n";

  // S4: Tuner 동작
  tuner->setCH("10");
  out << "SetCHValid=" << tuner->getCurrentCH() << "\n";

  tuner->setCH("999"); // 예외 발생 시 메시지 출력됨
  out << "AfterInvalidSetCH=" << tuner->getCurrentCH() << "\n";

  tuner->setCH("98");
  out << "SeekCH=" << tuner->seekCH() << "," << tuner->seekCH() << ","
      << tuner->seekCH() << "\n";
  tuner->setCH("42");
  out << "GetCurrentCH=" << tuner->getCurrentCH() << "\n";

  // ✅ verify 실행 (항상 도달)
  ApprovalTests::Approvals::verify(out.str());
}
