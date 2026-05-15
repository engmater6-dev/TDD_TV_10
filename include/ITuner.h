// ── ITuner.h — 순수 가상 인터페이스 (외부 제공, 수정 불가) ──
#pragma once
#include <string>
class ITuner {
public:
  virtual ~ITuner() = default;
  virtual std::string seekCH() = 0;
  virtual void setCH(std::string ch) = 0;
  virtual std::string getCurrentCH() = 0;
};
