#ifndef __ADH_CLOCK_H__
#define __ADH_CLOCK_H__

namespace engine
{
  class Clock
  {
  public:
    virtual ~Clock() = default;
    virtual float getTimestamp() const = 0;
  };
}

#endif
