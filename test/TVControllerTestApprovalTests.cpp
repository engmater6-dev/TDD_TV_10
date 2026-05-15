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
  std::string seekCH() override { return ""; }
  std::string getCurrentCH() override { return ""; }
};

TEST(TVControllerApprovalTest, VerifyAllScenarios) {
  std::vector<std::string> outputs;

  // S1-1: 한 자리 입력 + OK
  {
    FakeTunerForApproval tuner;
    TVController ctrl(tuner); // 수정된 부분
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
    TVController ctrl(tuner); // 수정된 부분
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
    TVController ctrl(tuner); // 수정된 부분
    std::ostringstream oss;
    auto oldBuf = std::cout.rdbuf(oss.rdbuf());
    ctrl.pushButton(remoteKey::KEY_1);
    ctrl.pushButton(remoteKey::KEY_2);
    ctrl.pushButton(remoteKey::KEY_3);
    ctrl.pushButton(remoteKey::KEY_OK); // OK 눌러야 확정
    std::cout.rdbuf(oldBuf);
    outputs.push_back("ThreeDigitInput:\n" + oss.str());
  }

  // S1-4: Leading zero (01 → 1)
  {
    FakeTunerForApproval tuner;
    TVController ctrl(tuner); // 수정된 부분
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
    TVController ctrl(tuner); // 수정된 부분
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
    TVController ctrl(tuner); // 수정된 부분
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
    TVController ctrl(tuner); // 수정된 부분
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
    TVController ctrl(tuner); // 수정된 부분
    std::ostringstream oss;
    auto oldBuf = std::cout.rdbuf(oss.rdbuf());
    ctrl.pushButton(remoteKey::KEY_0);
    ctrl.pushButton(remoteKey::KEY_9);
    std::cout.rdbuf(oldBuf);
    outputs.push_back("Boundary09:\n" + oss.str());
  }

  ApprovalTests::Approvals::verifyAll("TVController scenarios", outputs);
}
