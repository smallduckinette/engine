#include "engine/core/rtclock.h"

engine::RtClock::RtClock():
  _dawnOfTimes(std::chrono::steady_clock::now())
{
}

float engine::RtClock::getTimestamp() const
{
  auto now = std::chrono::steady_clock::now();
  return std::chrono::duration<float>(now - _dawnOfTimes).count();
}
