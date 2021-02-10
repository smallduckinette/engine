#ifndef __ENTITY_STRONGID_H__
#define __ENTITY_STRONGID_H__

#include <ostream>

namespace engine
{
  template <typename D, typename T>
  class StrongId
  {
  public:
    typedef D DiscriminantT;
    typedef T UnderlyingTypeT;

    StrongId():
      _value(T())
    {
    }

    StrongId(const T & value):
      _value(value)
    {
    }

    StrongId(const StrongId<D, T> & other):
      _value(other._value)
    {
    }

    StrongId<D, T> & operator=(const StrongId<D, T> & other)
    {
      _value = other._value;
      return *this;
    }

    bool operator==(const StrongId<D, T> & other) const
    {
      return (_value == other._value);
    }

    bool operator!=(const StrongId<D, T> & other) const
    {
      return (_value != other._value);
    }

    bool operator<(const StrongId<D, T> & other) const
    {
      return (_value < other._value);
    }

    T getValue() const
    {
      return _value;
    }

  private:
    T _value;
  };
  template<typename D, typename T>
  std::ostream & operator<<(std::ostream & os, const StrongId<D, T> & id)
  {
    return os << id.getValue();
  }

  template<typename T>
  class StrongIdGenerator;

  template <typename D, typename T>
  class StrongIdGenerator<StrongId<D, T> >
  {
  public:
    StrongIdGenerator(const T & init = T()):
      _nextValue(init)
    {
    }

    StrongId<D, T> generate()
    {
      return StrongId<D, T>(_nextValue++);
    }

  private:
    T _nextValue;
  };
}

namespace std
{
  template<typename D, typename T>
  struct hash<engine::StrongId<D, T> >
  {
    std::size_t operator()(const engine::StrongId<D, T> & id) const
    {
      return std::hash<T>{}(id.getValue());
    }
  };
}

#endif
