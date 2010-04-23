#include <Coordinate.h>
#include <iostream>
#define BOOST_TEST_MODULE Coordinate
#include <boost/test/unit_test.hpp>

using namespace geojackal;
using namespace std;

BOOST_AUTO_TEST_CASE(struct_Angle) {
  Angle a(52.0, 76.0, 93.25);
  BOOST_CHECK_CLOSE(a.deg(), 53.292569, 0.000001f);
  BOOST_CHECK_CLOSE(a.min(), 17.554166, 0.00001f);
  BOOST_CHECK_CLOSE(a.sec(), 33.25, 0.0001f);

  Angle b;
  b = 53.292569;
  BOOST_CHECK_CLOSE(b.deg(), 53.292569, 0.000001f);
  BOOST_CHECK_CLOSE(b.min(), 17.55414, 0.00001f);
  BOOST_CHECK_CLOSE(b.sec(), 33.2484, 0.0001f);
}
