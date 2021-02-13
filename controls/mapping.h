#ifndef __CONTROLS_MAPPING_H__
#define __CONTROLS_MAPPING_H__

namespace engine
{
  template<typename FROM, typename TO>
  class Mapping
  {
  public:
    Mapping(FROM fmin, FROM fmax, TO tmin, TO tmax):
      _fmin(fmin),
      _fmax(fmax),
      _tmin(tmin),
      _tmax(tmax),
      _direct(true)
    {
      using std::swap;
      if(_fmin > _fmax)
      {
        swap(_fmin, _fmax);
        _direct = !_direct;
      }
      if(_tmin > _tmax)
      {
        swap(_tmin, _tmax);
        _direct = !_direct;
      }
    };

    TO map(FROM value) const
    {
      return
        _direct ?
        (std::clamp(value, _fmin, _fmax) - _fmin) * (_tmax - _tmin) / (_fmax - _fmin) + _tmin :
        (_tmax - _tmin) - (std::clamp(value, _fmin, _fmax) - _fmin) * (_tmax - _tmin) / (_fmax - _fmin) + _tmin;
    };

  private:
    FROM _fmin;
    FROM _fmax;
    TO _tmin;
    TO _tmax;
    bool _direct;
  };
}

#endif
