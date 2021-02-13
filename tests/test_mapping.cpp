#include <boost/test/unit_test.hpp>

#include "engine/controls/mapping.h"

BOOST_AUTO_TEST_SUITE(Mapping)

BOOST_AUTO_TEST_CASE(testSimpleMapping)
{
  engine::Mapping<int16_t, float> mapping(-32765, 32765, -1, 1);

  BOOST_CHECK_CLOSE(0, mapping.map(0), 0.1);
  BOOST_CHECK_CLOSE(1, mapping.map(32765), 0.1);
  BOOST_CHECK_CLOSE(-1, mapping.map(-32765), 0.1);
  BOOST_CHECK_CLOSE(0.24416, mapping.map(8000), 0.1);
}

BOOST_AUTO_TEST_CASE(testClamping)
{
  engine::Mapping<int16_t, float> mapping(-10000, 10000, -1, 1);

  BOOST_CHECK_CLOSE(0, mapping.map(0), 0.1);
  BOOST_CHECK_CLOSE(1, mapping.map(32765), 0.1);
  BOOST_CHECK_CLOSE(-1, mapping.map(-32765), 0.1);
  BOOST_CHECK_CLOSE(0.5, mapping.map(5000), 0.1);
}

BOOST_AUTO_TEST_CASE(testInverse)
{
  engine::Mapping<int16_t, float> mapping(10000, -10000, 0, 1);

  BOOST_CHECK_CLOSE(0, mapping.map(10000), 0.1);
  BOOST_CHECK_CLOSE(1, mapping.map(-10000), 0.1);
  BOOST_CHECK_CLOSE(0.5, mapping.map(0), 0.1);
}

BOOST_AUTO_TEST_CASE(testInverse2)
{
  engine::Mapping<int16_t, float> mapping(-5000, 10000, 2, 1);

  BOOST_CHECK_CLOSE(2, mapping.map(-5000), 0.1);
  BOOST_CHECK_CLOSE(1, mapping.map(10000), 0.1);
  BOOST_CHECK_CLOSE(1.6666, mapping.map(0), 0.1);
}

BOOST_AUTO_TEST_SUITE_END()
