#ifndef __ADH_CLOCK_H__
#define __ADH_CLOCK_H__

namespace engine::adh
{
  class Clock
  {
  public:
    virtual ~Clock() = default;
    virtual float getTimestamp() const = 0;
  };
}

#endif
