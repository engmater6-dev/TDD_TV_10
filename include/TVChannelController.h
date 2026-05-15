#pragma once
#include "ITuner.h"
#include <algorithm>
#include <string>
#include <vector>

class TVChannelController {
  ITuner &tuner_;              // 인터페이스 참조 주입
  int inputBuffer_ = -1;       // -1: 버퍼 비어있음
  std::vector<int> favorites_; // 즐겨찾기 목록 (정렬 유지)

public:
  explicit TVChannelController(ITuner &t) : tuner_(t) {}

  // 리모컨 입력 처리 메서드
  void pressNumber(int digit); // 숫자 버튼
  void pressConfirm();         // 확인 버튼
  void pressFavorite();        // 현재 채널 즐겨찾기 토글
  void pressNextFavorite();    // 다음 즐겨찾기 이동
  void pressOther();           // 기타 버튼 → 버퍼 무효화
  void pressUp();              // 채널 업 (전체 채널 기준)
  void pressDown();            // 채널 다운 (전체 채널 기준)
  void pressUpFavorite();      // 채널 업 (즐겨찾기 기준)
  void pressDownFavorite();    // 채널 다운 (즐겨찾기 기준)

  // 즐겨찾기 목록 조회
  const std::vector<int> &getFavoriteChannels() const { return favorites_; }

  // 즐겨찾기 추가 (중복 방지 + 정렬 유지)
  void addFavorite(int ch) {
    if (!isFavorite(ch)) {
      favorites_.push_back(ch);
      std::sort(favorites_.begin(), favorites_.end());
    }
  }

private:
  bool isValidChannel(int ch) const { return ch >= 0 && ch <= 99; }
  bool isFavorite(int ch) const {
    return std::find(favorites_.begin(), favorites_.end(), ch) !=
           favorites_.end();
  }
  void applyChannel(int ch); // 채널 적용 (유효성 검증 + tuner 호출)
};
