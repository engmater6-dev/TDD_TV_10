#pragma once
#include "ITuner.h"
#include <string>
#include <vector>


class TVChannelController {
  ITuner &tuner_;
  std::vector<int> buffer_;           // ✅ 숫자 입력 버퍼
  std::vector<int> favoriteChannels_; // ✅ 즐겨찾기 채널 목록

public:
  explicit TVChannelController(ITuner &tuner) : tuner_(tuner) {}

  // 숫자 입력 및 확인
  void pressNumber(int num);
  void pressConfirm();

  // 즐겨찾기 관련
  void pressFavorite();
  void pressNextFavorite();
  void pressUpFavorite();
  void pressDownFavorite();

  // 채널 이동
  void pressUp();
  void pressDown();

  // 기타 입력 처리
  void pressOther();

  // 내부 동작
  void applyChannel(int ch);
  bool isValidChannel(int ch) const; // ✅ const 유지

  // 즐겨찾기 목록 조회 (불변 참조 반환)
  const std::vector<int> &getFavoriteChannels() const {
    return favoriteChannels_;
  }
};
