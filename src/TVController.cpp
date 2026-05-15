#include "TVController.h"
#include "remoteKey.h"

// 숫자 버튼 채널 변경 (To Do List 1번)
void TVController::pushButton(remoteKey key) {
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
    controller_.pressConfirm(); // 1-1: 한자리 입력 + 확인
    break;

  case remoteKey::KEY_FAVORITE:
    controller_.pressFavorite(); // To Do List 2번
    break;

  case remoteKey::KEY_NEXT_FAVORITE:
    controller_.pressNextFavorite(); // To Do List 3번
    break;

  case remoteKey::KEY_OTHER:
    controller_.pressOther(); // 버퍼 무효화
    break;
  }
}
