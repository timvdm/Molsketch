#include <cxxtest/TestSuite.h>
#include <element.h>

using namespace Molsketch;

class ValenceTest : public CxxTest::TestSuite
{
public:
  void testExpectedValences()
  {
    TS_ASSERT_EQUALS(expectedValence(Element::H), 1);
//    TS_ASSERT_EQUALS(expectedValence(Element::H), 1);

//    QCOMPARE( expectedValence(Element::H), 1 );
//    QCOMPARE( expectedValence(Element::K), 1 );

//    QCOMPARE( expectedValence(Element::Mg), 2 );
//    QCOMPARE( expectedValence(Element::Ca), 2 );

//    QCOMPARE( expectedValence(Element::B), 3 );
//    QCOMPARE( expectedValence(Element::Al), 3 );

//    QCOMPARE( expectedValence(Element::C), 4 );
//    QCOMPARE( expectedValence(Element::Si), 4 );

//    QCOMPARE( expectedValence(Element::N), 3 );
//    QCOMPARE( expectedValence(Element::P), 3 );

//    QCOMPARE( expectedValence(Element::O), 2 );
//    QCOMPARE( expectedValence(Element::S), 2 );

//    QCOMPARE( expectedValence(Element::F), 1 );
//    QCOMPARE( expectedValence(Element::Cl), 1 );

//    QCOMPARE( expectedValence(Element::He), 0 );
//    QCOMPARE( expectedValence(Element::Ne), 0 );
  }
};
