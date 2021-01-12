#include "rtclock.h"

engine::adh::RtClock::RtClock():
  _dawnOfTimes(std::chrono::steady_clock::now())
{
}

float engine::adh::RtClock::getTimestamp() const
{
  auto now = std::chrono::steady_clock::now();
  return std::chrono::duration<float>(now - _dawnOfTimes).count();
}
