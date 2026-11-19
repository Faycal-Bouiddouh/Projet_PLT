#include <boost/test/unit_test.hpp>

#include "../../src/shared/state/Plate.h"
#include "../../src/shared/state/L.h"
#include "../../src/shared/state/T.h"
#include "../../src/shared/state/I.h"
#include "../../src/shared/state/Board.h"

using namespace state;

BOOST_AUTO_TEST_CASE(Getter_Plate)
{
    L plate;
    BOOST_CHECK(plate.getNorth());
    BOOST_CHECK(plate.getEast());
    BOOST_CHECK(!plate.getSouth());
    BOOST_CHECK(!plate.getWest());

    T plate2;
    BOOST_CHECK(!plate2.getNorth());
    BOOST_CHECK(plate2.getEast());
    BOOST_CHECK(plate2.getSouth());
    BOOST_CHECK(plate2.getWest());
    
    I plate3;
    BOOST_CHECK(plate3.getNorth());
    BOOST_CHECK(!plate3.getEast());
    BOOST_CHECK(plate3.getSouth());
    BOOST_CHECK(!plate3.getWest()); 
}

BOOST_AUTO_TEST_CASE(TestFourTurnsIsIdentity)
{
  T plate;
 
  for (int k = 0; k < 4; ++k) {
    plate.turnPlate();
  }
  BOOST_CHECK(!plate.getNorth() && plate.getEast() &&
              plate.getSouth() && plate.getWest());
  L plate2;
  for (int k = 0; k < 4; ++k) {
    plate2.turnPlate();
  }
  BOOST_CHECK(plate2.getNorth() && plate2.getEast() &&
              !plate2.getSouth() && !plate2.getWest());

  I plate3;
  for (int k = 0; k < 4; ++k) {
    plate3.turnPlate();
  }
  BOOST_CHECK(plate3.getNorth() && !plate3.getEast() &&
              plate3.getSouth() && !plate3.getWest());
  
}

BOOST_AUTO_TEST_CASE(Test_TurnPlate)
{
  L plate;
  plate.turnPlate();
  BOOST_CHECK(1);
}



