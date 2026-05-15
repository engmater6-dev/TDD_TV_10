#define APPROVALS_GOOGLETEST
#include "ITuner.h"
#include "TVController.h"
#include "remoteKey.h"
#include <ApprovalTests.hpp>
#include <gtest/gtest.h>
#include <sstream>

// 간단한 MockTuner: setCH 호출 시 출력만 남김
class MockTunerForApproval : public ITuner {
public:
  void setCH(std::string ch) override {
    std::cout << "setCH called with: " << ch << std::endl;
  }
  std::string seekCH() override { return ""; }
  std::string getCurrentCH() override { return ""; }
};

// 여러 시나리오를 하나의 승인 파일로 합치기
TEST(TVControllerApprovalTest, CombinedVerifyAll) {
  std::vector<std::string> outputs;

  // 1-1: One digit + OK
  {
    MockTunerForApproval tuner;
    TVController controller(&tuner);
    std::ostringstream oss;
    auto oldBuf = std::cout.rdbuf(oss.rdbuf());
    controller.pushButton(remoteKey::KEY_1);
    controller.pushButton(remoteKey::KEY_OK);
    std::cout.rdbuf(oldBuf);
    outputs.push_back("OneDigitWithOk:\n" + oss.str());
  }

  // 1-2: Two digit auto set
  {
    MockTunerForApproval tuner;
    TVController controller(&tuner);
    std::ostringstream oss;
    auto oldBuf = std::cout.rdbuf(oss.rdbuf());
    controller.pushButton(remoteKey::KEY_1);
    controller.pushButton(remoteKey::KEY_2);
    std::cout.rdbuf(oldBuf);
    outputs.push_back("TwoDigitAutoSetCh:\n" + oss.str());
  }

  // 1-3: Three digit input (예: 123 → 현재는 12 → 3 처리됨)
  {
    MockTunerForApproval tuner;
    TVController controller(&tuner);
    std::ostringstream oss;
    auto oldBuf = std::cout.rdbuf(oss.rdbuf());
    controller.pushButton(remoteKey::KEY_1);
    controller.pushButton(remoteKey::KEY_2);
    controller.pushButton(remoteKey::KEY_3);
    std::cout.rdbuf(oldBuf);
    outputs.push_back("ThreeDigitInput:\n" + oss.str());
  }

  // 1-4: Leading zero two digit (예: 01 → 채널 1)
  {
    MockTunerForApproval tuner;
    TVController controller(&tuner);
    std::ostringstream oss;
    auto oldBuf = std::cout.rdbuf(oss.rdbuf());
    controller.pushButton(remoteKey::KEY_0);
    controller.pushButton(remoteKey::KEY_1);
    std::cout.rdbuf(oldBuf);
    outputs.push_back("LeadingZeroTwoDigit:\n" + oss.str());
  }

  ApprovalTests::Approvals::verifyAll("TVController scenarios", outputs);
}
