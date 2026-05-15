#define APPROVALS_GOOGLETEST
#include "ITuner.h"
#include "TVController.h"
#include "remoteKey.h"
#include <ApprovalTests.hpp>
#include <gtest/gtest.h>
#include <sstream>

// 간단한 FakeTuner: setCH 호출 시 출력만 남김
class FakeTunerForApproval : public ITuner {
public:
  void setCH(std::string ch) override {
    std::cout << "setCH called with: " << ch << std::endl;
  }
  std::string seekCH() override {
    return "7";
  } // To Do list 4번: 검색 결과 Mock
  std::string getCurrentCH() override { return "5"; }
};

TEST(TVControllerApprovalTest, VerifyAllScenarios) {
  std::vector<std::string> outputs;

  // S1-1: 한 자리 입력 + OK
  {
    FakeTunerForApproval tuner;
    TVController ctrl(tuner);
    std::ostringstream oss;
    auto oldBuf = std::cout.rdbuf(oss.rdbuf());
    ctrl.pushButton(remoteKey::KEY_1);
    ctrl.pushButton(remoteKey::KEY_OK);
    std::cout.rdbuf(oldBuf);
    outputs.push_back("OneDigitWithOk:\n" + oss.str());
  }

  // S1-2: 두 자리 자동 변경
  {
    FakeTunerForApproval tuner;
    TVController ctrl(tuner);
    std::ostringstream oss;
    auto oldBuf = std::cout.rdbuf(oss.rdbuf());
    ctrl.pushButton(remoteKey::KEY_1);
    ctrl.pushButton(remoteKey::KEY_2);
    std::cout.rdbuf(oldBuf);
    outputs.push_back("TwoDigitAutoSetCh:\n" + oss.str());
  }

  // S1-3: 세 자리 입력 (123 → 12 → 3)
  {
    FakeTunerForApproval tuner;
    TVController ctrl(tuner);
    std::ostringstream oss;
    auto oldBuf = std::cout.rdbuf(oss.rdbuf());
    ctrl.pushButton(remoteKey::KEY_1);
    ctrl.pushButton(remoteKey::KEY_2);
    ctrl.pushButton(remoteKey::KEY_3);
    ctrl.pushButton(remoteKey::KEY_OK);
    std::cout.rdbuf(oldBuf);
    outputs.push_back("ThreeDigitInput:\n" + oss.str());
  }

  // S1-4: Leading zero (01 → 1)
  {
    FakeTunerForApproval tuner;
    TVController ctrl(tuner);
    std::ostringstream oss;
    auto oldBuf = std::cout.rdbuf(oss.rdbuf());
    ctrl.pushButton(remoteKey::KEY_0);
    ctrl.pushButton(remoteKey::KEY_1);
    std::cout.rdbuf(oldBuf);
    outputs.push_back("LeadingZeroTwoDigit:\n" + oss.str());
  }

  // S1-5: 네 자리 입력 (1234 → 12 → 34)
  {
    FakeTunerForApproval tuner;
    TVController ctrl(tuner);
    std::ostringstream oss;
    auto oldBuf = std::cout.rdbuf(oss.rdbuf());
    ctrl.pushButton(remoteKey::KEY_1);
    ctrl.pushButton(remoteKey::KEY_2);
    ctrl.pushButton(remoteKey::KEY_3);
    ctrl.pushButton(remoteKey::KEY_4);
    std::cout.rdbuf(oldBuf);
    outputs.push_back("FourDigitInput:\n" + oss.str());
  }

  // S1-6: 경계값 99
  {
    FakeTunerForApproval tuner;
    TVController ctrl(tuner);
    std::ostringstream oss;
    auto oldBuf = std::cout.rdbuf(oss.rdbuf());
    ctrl.pushButton(remoteKey::KEY_9);
    ctrl.pushButton(remoteKey::KEY_9);
    std::cout.rdbuf(oldBuf);
    outputs.push_back("Boundary99:\n" + oss.str());
  }

  // S1-7: 경계값 00
  {
    FakeTunerForApproval tuner;
    TVController ctrl(tuner);
    std::ostringstream oss;
    auto oldBuf = std::cout.rdbuf(oss.rdbuf());
    ctrl.pushButton(remoteKey::KEY_0);
    ctrl.pushButton(remoteKey::KEY_0);
    std::cout.rdbuf(oldBuf);
    outputs.push_back("Boundary00:\n" + oss.str());
  }

  // S1-8: 경계값 09
  {
    FakeTunerForApproval tuner;
    TVController ctrl(tuner);
    std::ostringstream oss;
    auto oldBuf = std::cout.rdbuf(oss.rdbuf());
    ctrl.pushButton(remoteKey::KEY_0);
    ctrl.pushButton(remoteKey::KEY_9);
    std::cout.rdbuf(oldBuf);
    outputs.push_back("Boundary09:\n" + oss.str());
  }

  // S1-9: seekCH, getCurrentCH 호출 테스트
  {
    FakeTunerForApproval tuner;
    TVController ctrl(tuner);
    std::ostringstream oss;
    auto oldBuf = std::cout.rdbuf(oss.rdbuf());
    ctrl.pushButton(remoteKey::KEY_1);
    ctrl.pushButton(remoteKey::KEY_OK);
    std::string seekResult = tuner.seekCH();
    std::string currentResult = tuner.getCurrentCH();
    std::cout << "seekCH returned: " << seekResult << std::endl;
    std::cout << "getCurrentCH returned: " << currentResult << std::endl;
    std::cout.rdbuf(oldBuf);
    outputs.push_back("SeekAndGetCurrentCH:\n" + oss.str());
  }

  // S2-1: 선호 채널 추가/삭제
  {
    FakeTunerForApproval tuner;
    TVController ctrl(tuner);
    std::ostringstream oss;
    auto oldBuf = std::cout.rdbuf(oss.rdbuf());
    ctrl.pushButton(remoteKey::KEY_5);
    ctrl.pushButton(remoteKey::KEY_OK);
    ctrl.pushButton(remoteKey::KEY_FAVORITE);
    ctrl.pushButton(remoteKey::KEY_FAVORITE); // 다시 눌러 삭제
    std::cout.rdbuf(oldBuf);
    outputs.push_back("ToggleFavorite:\n" + oss.str());
  }

  // S3-1: 선호 채널 이동
  {
    FakeTunerForApproval tuner;
    TVController ctrl(tuner);
    std::ostringstream oss;
    auto oldBuf = std::cout.rdbuf(oss.rdbuf());
    ctrl.pushButton(remoteKey::KEY_1);
    ctrl.pushButton(remoteKey::KEY_OK);
    ctrl.pushButton(remoteKey::KEY_FAVORITE);
    ctrl.pushButton(remoteKey::KEY_9);
    ctrl.pushButton(remoteKey::KEY_OK);
    ctrl.pushButton(remoteKey::KEY_FAVORITE);
    ctrl.pushButton(remoteKey::KEY_1);
    ctrl.pushButton(remoteKey::KEY_OK);
    ctrl.pushButton(remoteKey::KEY_NEXT_FAVORITE);
    std::cout.rdbuf(oldBuf);
    outputs.push_back("NextFavorite:\n" + oss.str());
  }

  // S5: 업/다운 동작 (전체 채널 기준)
  {
    FakeTunerForApproval tuner;
    TVController ctrl(tuner);
    std::ostringstream oss;
    auto oldBuf = std::cout.rdbuf(oss.rdbuf());
    ctrl.pushButton(remoteKey::KEY_0);
    ctrl.pushButton(remoteKey::KEY_OK);
    ctrl.pushButton(remoteKey::KEY_UP);
    ctrl.pushButton(remoteKey::KEY_DOWN);
    std::cout.rdbuf(oldBuf);
    outputs.push_back("ChannelUpDown:\n" + oss.str());
  }

  // S6: 업/다운 동작 (즐겨찾기 기준)
  {
    FakeTunerForApproval tuner;
    TVController ctrl(tuner);
    std::ostringstream oss;
    auto oldBuf = std::cout.rdbuf(oss.rdbuf());
    ctrl.pushButton(remoteKey::KEY_2);
    ctrl.pushButton(remoteKey::KEY_OK);
    ctrl.pushButton(remoteKey::KEY_FAVORITE);
    ctrl.pushButton(remoteKey::KEY_5);
    ctrl.pushButton(remoteKey::KEY_OK);
    ctrl.pushButton(remoteKey::KEY_FAVORITE);
    ctrl.pushButton(remoteKey::KEY_2);
    ctrl.pushButton(remoteKey::KEY_OK);
    ctrl.pushButton(remoteKey::KEY_UP_FAVORITE);
    ctrl.pushButton(remoteKey::KEY_DOWN_FAVORITE);
    std::cout.rdbuf(oldBuf);
    outputs.push_back("ChannelUpDownFavorite:\n" + oss.str());
  }

  ApprovalTests::Approvals::verifyAll("TVController scenarios", outputs);
}
