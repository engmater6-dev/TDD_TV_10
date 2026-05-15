/**
 * Copyright 2020 by Samsung Electronics, Inc.,
 *
 * This software is the confidential and proprietary information
 * of Samsung Electronics, Inc. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */

#ifndef TV_CONTROLLER_H
#define TV_CONTROLLER_H

#include "ITuner.h"
#include "remoteKey.h"
#include <iostream>
#include <string>

class TVController {
private:
  ITuner *tuner;
  std::string processingCH;

  void setTunerCh() {
    // 로그는 테스트의 결과가 절대 아닙니다. 로그가 있는 것을 테스트로
    // 간주하지
    // 마시기 바랍니다.
    std::cout << "현재 설정하는 채널 : " << processingCH << std::endl;
    tuner->setCH(processingCH);
  }

public:
  explicit TVController(ITuner *tuner) : tuner(tuner), processingCH("") {}

  void pushButton(remoteKey key) {
    switch (key) {
    case remoteKey::KEY_0:
      processingCH += "0";
      break;
    case remoteKey::KEY_1:
      processingCH += "1";
      break;
    case remoteKey::KEY_2:
      processingCH += "2";
      break;
    case remoteKey::KEY_3:
      processingCH += "3";
      break;
    case remoteKey::KEY_4:
      processingCH += "4";
      break;
    case remoteKey::KEY_5:
      processingCH += "5";
      break;
    case remoteKey::KEY_6:
      processingCH += "6";
      break;
    case remoteKey::KEY_7:
      processingCH += "7";
      break;
    case remoteKey::KEY_8:
      processingCH += "8";
      break;
    case remoteKey::KEY_9:
      processingCH += "9";
      break;
    case remoteKey::KEY_OK:
      setTunerCh();
      processingCH.clear();
      return;
    }

    if (processingCH.size() == 2) {
      setTunerCh();
      processingCH.clear();
    }
  }
};

#endif // TV_CONTROLLER_H
