#include "TVController.h"

void TVController::setTunerCh() {
  if (!processingCH.empty()) {
    tuner->setCH(processingCH); // ITuner 인터페이스 호출
    std::cout << "현재 설정하는 채널 : " << processingCH << std::endl;
  }
}

void TVController::pushButton(remoteKey key) {
  switch (key) {
  case remoteKey::KEY_0:
    processingCH = "0";
    break;
  case remoteKey::KEY_1:
    processingCH = "1";
    break;
  case remoteKey::KEY_2:
    processingCH = "2";
    break;
  case remoteKey::KEY_3:
    processingCH = "3";
    break;
  case remoteKey::KEY_4:
    processingCH = "4";
    break;
  case remoteKey::KEY_5:
    processingCH = "5";
    break;
  case remoteKey::KEY_6:
    processingCH = "6";
    break;
  case remoteKey::KEY_7:
    processingCH = "7";
    break;
  case remoteKey::KEY_8:
    processingCH = "8";
    break;
  case remoteKey::KEY_9:
    processingCH = "9";
    break;

  case remoteKey::KEY_OK:
    // 확인 버튼 → 현재 버퍼를 채널로 설정
    setTunerCh();
    processingCH.clear();
    break;

  default:
    break;
  }
}
