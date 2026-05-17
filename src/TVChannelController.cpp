#include "TVChannelController.h"
#include <algorithm>
#include <stdexcept>

int TVChannelController::getCurrentChannel() const {
  return std::stoi(tuner_.getCurrentCH());
}

// 숫자 입력 처리
void TVChannelController::pressNumber(int num) {
  buffer_.push_back(num);

  if (buffer_.size() == 2) {
    int ch = buffer_[0] * 10 + buffer_[1];
    applyChannel(ch);
    buffer_.clear();
  } else if (buffer_.size() > 2) {
    int ch = buffer_[buffer_.size() - 2] * 10 + buffer_[buffer_.size() - 1];
    applyChannel(ch);
    buffer_.clear();
  }
}

void TVChannelController::pressConfirm() {
  if (buffer_.size() == 1) {
    applyChannel(buffer_[0]);
  }
  buffer_.clear();
}

void TVChannelController::pressFavorite() {
  int current = getCurrentChannel();
  auto it =
      std::find(favoriteChannels_.begin(), favoriteChannels_.end(), current);
  if (it == favoriteChannels_.end()) {
    favoriteChannels_.push_back(current);
    std::sort(favoriteChannels_.begin(), favoriteChannels_.end());
  } else {
    favoriteChannels_.erase(it);
  }
}

void TVChannelController::pressNextFavorite() {
  pressUpFavorite(); // ✅ 중복 제거: 동일 로직 재사용
}

void TVChannelController::pressUp() {
  int next = (getCurrentChannel() + 1) % (MAX_CHANNEL + 1);
  applyChannel(next);
}

void TVChannelController::pressDown() {
  int current = getCurrentChannel();
  int next = (current == MIN_CHANNEL) ? MAX_CHANNEL : current - 1;
  applyChannel(next);
}

void TVChannelController::pressUpFavorite() {
  if (favoriteChannels_.empty())
    return;
  int current = getCurrentChannel();
  auto it = std::find_if(favoriteChannels_.begin(), favoriteChannels_.end(),
                         [&](int ch) { return ch > current; });
  int next = (it != favoriteChannels_.end()) ? *it : favoriteChannels_.front();
  applyChannel(next);
}

void TVChannelController::pressDownFavorite() {
  if (favoriteChannels_.empty())
    return;
  int current = getCurrentChannel();
  auto it = std::find_if(favoriteChannels_.rbegin(), favoriteChannels_.rend(),
                         [&](int ch) { return ch < current; });
  int next = (it != favoriteChannels_.rend()) ? *it : favoriteChannels_.back();
  applyChannel(next);
}

void TVChannelController::pressOther() { buffer_.clear(); }

void TVChannelController::applyChannel(int ch) {
  if (!isValidChannel(ch)) {
    throw std::invalid_argument("채널 범위 초과: " + std::to_string(ch));
  }
  tuner_.setCH(std::to_string(ch));
}

bool TVChannelController::isValidChannel(int ch) const {
  return ch >= MIN_CHANNEL && ch <= MAX_CHANNEL;
}
