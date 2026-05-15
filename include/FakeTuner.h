#pragma once
#include "ITuner.h"
#include <algorithm>
#include <stdexcept>
#include <vector>

class FakeTuner : public ITuner {
  int current_ = 0;
  std::vector<int> available_;

public:
  explicit FakeTuner(std::vector<int> avail) : available_(std::move(avail)) {
    if (available_.empty()) {
      // 기본 채널 목록이 없으면 0~99 전체 채널을 기본값으로 설정
      for (int i = 0; i <= 99; ++i) {
        available_.push_back(i);
      }
    }
  }

  // 채널 검색: 현재보다 큰 값 중 최소, 없으면 wrap-around
  std::string seekCH() override {
    auto it = std::find_if(available_.begin(), available_.end(),
                           [&](int ch) { return ch > current_; });
    current_ = (it != available_.end()) ? *it : available_.front();
    return std::to_string(current_);
  }

  // 채널 설정: 0~99 범위 검증
  void setCH(std::string ch) override {
    int v = std::stoi(ch);
    if (v < 0 || v > 99) {
      throw std::invalid_argument("채널 범위 초과: " + ch);
    }
    current_ = v;
  }

  // 현재 채널 반환
  std::string getCurrentCH() override { return std::to_string(current_); }
};
