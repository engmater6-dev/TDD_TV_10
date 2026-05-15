#include "TVChannelController.h"
#include <algorithm>
#include <stdexcept>


void TVChannelController::pressNumber(int digit) {
  if (inputBuffer_ == -1) {
    inputBuffer_ = digit;
  } else {
    int ch = inputBuffer_ * 10 + digit;
    inputBuffer_ = -1;
    applyChannel(ch);
  }
}

void TVChannelController::pressConfirm() {
  if (inputBuffer_ != -1) {
    int ch = inputBuffer_;
    inputBuffer_ = -1;
    applyChannel(ch);
  }
}

void TVChannelController::pressOther() { inputBuffer_ = -1; }

// 2-1. 선호 채널 추가/삭제
void TVChannelController::pressFavorite() {
  int ch = std::stoi(tuner_.getCurrentCH());
  if (isFavorite(ch)) {
    favorites_.erase(std::remove(favorites_.begin(), favorites_.end(), ch),
                     favorites_.end());
  } else {
    addFavorite(ch);
  }
}

// 3-1. 선호 채널 이동
void TVChannelController::pressNextFavorite() {
  if (favorites_.empty())
    return;
  int cur = std::stoi(tuner_.getCurrentCH());
  auto it = std::upper_bound(favorites_.begin(), favorites_.end(), cur);
  int next = (it != favorites_.end()) ? *it : favorites_.front();
  applyChannel(next);
}

// 5-1. 채널 업 (저장 채널 없음)
void TVChannelController::pressUp() {
  int cur = std::stoi(tuner_.getCurrentCH());
  int next = (cur + 1 > 99) ? 0 : cur + 1;
  applyChannel(next);
}

// 5-2. 채널 다운 (저장 채널 없음)
void TVChannelController::pressDown() {
  int cur = std::stoi(tuner_.getCurrentCH());
  int next = (cur - 1 < 0) ? 99 : cur - 1;
  applyChannel(next);
}

// 6. 채널 업 (저장 채널 기준)
void TVChannelController::pressUpFavorite() {
  if (favorites_.empty()) {
    pressUp();
    return;
  }
  int cur = std::stoi(tuner_.getCurrentCH());
  auto it = std::upper_bound(favorites_.begin(), favorites_.end(), cur);
  int next = (it != favorites_.end()) ? *it : favorites_.front();
  applyChannel(next);
}

// 6. 채널 다운 (저장 채널 기준)
void TVChannelController::pressDownFavorite() {
  if (favorites_.empty()) {
    pressDown();
    return;
  }
  int cur = std::stoi(tuner_.getCurrentCH());
  auto it = std::lower_bound(favorites_.begin(), favorites_.end(), cur);
  if (it == favorites_.begin()) {
    applyChannel(favorites_.back());
  } else {
    --it;
    applyChannel(*it);
  }
}

// 채널 적용
void TVChannelController::applyChannel(int ch) {
  if (!isValidChannel(ch))
    throw std::invalid_argument("채널 범위 초과: " + std::to_string(ch));
  tuner_.setCH(std::to_string(ch));
}
