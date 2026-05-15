#pragma once
#include "ITuner.h"
#include "TVChannelController.h"
#include "remoteKey.h"

class TVController {
  TVChannelController controller_;

public:
  explicit TVController(ITuner &tuner) : controller_(tuner) {}

  void pushButton(remoteKey key) {
    switch (key) {
    case remoteKey::KEY_0:
      controller_.pressNumber(0);
      break;
    case remoteKey::KEY_1:
      controller_.pressNumber(1);
      break;
    case remoteKey::KEY_2:
      controller_.pressNumber(2);
      break;
    case remoteKey::KEY_3:
      controller_.pressNumber(3);
      break;
    case remoteKey::KEY_4:
      controller_.pressNumber(4);
      break;
    case remoteKey::KEY_5:
      controller_.pressNumber(5);
      break;
    case remoteKey::KEY_6:
      controller_.pressNumber(6);
      break;
    case remoteKey::KEY_7:
      controller_.pressNumber(7);
      break;
    case remoteKey::KEY_8:
      controller_.pressNumber(8);
      break;
    case remoteKey::KEY_9:
      controller_.pressNumber(9);
      break;

    case remoteKey::KEY_OK:
      controller_.pressConfirm();
      break;

    case remoteKey::KEY_FAVORITE:
      controller_.pressFavorite();
      break;

    case remoteKey::KEY_NEXT_FAVORITE:
      controller_.pressNextFavorite();
      break;

    case remoteKey::KEY_UP:
      controller_.pressUp();
      break;

    case remoteKey::KEY_DOWN:
      controller_.pressDown();
      break;

    case remoteKey::KEY_UP_FAVORITE:
      controller_.pressUpFavorite();
      break;

    case remoteKey::KEY_DOWN_FAVORITE:
      controller_.pressDownFavorite();
      break;

    case remoteKey::KEY_OTHER:
      controller_.pressOther();
      break;
    }
  }

  // ✅ 테스트에서 내부 컨트롤러 접근 가능하도록 getter 추가
  TVChannelController &getController() { return controller_; }
  const TVChannelController &getController() const { return controller_; }
};
