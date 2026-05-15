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
  case remoteKey::KEY_1:
  case remoteKey::KEY_2:
  case remoteKey::KEY_3:
  case remoteKey::KEY_4:
  case remoteKey::KEY_5:
  case remoteKey::KEY_6:
  case remoteKey::KEY_7:
  case remoteKey::KEY_8:
  case remoteKey::KEY_9:
    // 숫자 입력 → 버퍼에 저장 (한 자리만)
    processingCH = to_string(key);
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
