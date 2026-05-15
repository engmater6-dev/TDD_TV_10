#include "TVController.h"
#include "remoteKey.h"

// 리모컨 입력 처리
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
    controller_.pressFavorite(); // 2-1: 선호 채널 추가/삭제
    break;

  case remoteKey::KEY_NEXT_FAVORITE:
    controller_.pressNextFavorite(); // 3-1: 선호 채널 이동
    break;

  case remoteKey::KEY_UP:
    controller_.pressUp(); // 5-1: 채널 업 (전체 채널 기준)
    break;

  case remoteKey::KEY_DOWN:
    controller_.pressDown(); // 5-2: 채널 다운 (전체 채널 기준)
    break;

  case remoteKey::KEY_UP_FAVORITE:
    controller_.pressUpFavorite(); // 6: 채널 업 (즐겨찾기 기준)
    break;

  case remoteKey::KEY_DOWN_FAVORITE:
    controller_.pressDownFavorite(); // 6: 채널 다운 (즐겨찾기 기준)
    break;

  case remoteKey::KEY_OTHER:
    controller_.pressOther(); // 버퍼 무효화
    break;
  }
}
