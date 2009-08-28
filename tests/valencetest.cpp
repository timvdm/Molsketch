#include <QObject>
#include <QtTest>

#include <molsketch/element.h>

using namespace Molsketch;

class ValenceTest : public QObject
{
  Q_OBJECT

  private slots:
    /**
     * Called before the first test function is executed.
     */
    void initTestCase();

    /**
     * Called after the last test function is executed.
     */
    void cleanupTestCase();

    /**
     * Called before each test function is executed.
     */
    void init();

    /**
     * Called after every test function.
     */
    void cleanup();

    void elementGroups();
    void expectedValences();
    void valenceElectrons();

};

void ValenceTest::initTestCase()
{
}

void ValenceTest::cleanupTestCase()
{
}

void ValenceTest::init()
{
}

void ValenceTest::cleanup()
{
}

void ValenceTest::expectedValences()
{
  QCOMPARE( expectedValence(Element::H), 1 );
  QCOMPARE( expectedValence(Element::K), 1 );
  
  QCOMPARE( expectedValence(Element::Mg), 2 );
  QCOMPARE( expectedValence(Element::Ca), 2 );
  
  QCOMPARE( expectedValence(Element::B), 3 );
  QCOMPARE( expectedValence(Element::Al), 3 );
  
  QCOMPARE( expectedValence(Element::C), 4 );
  QCOMPARE( expectedValence(Element::Si), 4 );

  QCOMPARE( expectedValence(Element::N), 3 );
  QCOMPARE( expectedValence(Element::P), 3 );
  
  QCOMPARE( expectedValence(Element::O), 2 );
  QCOMPARE( expectedValence(Element::S), 2 );
  
  QCOMPARE( expectedValence(Element::F), 1 );
  QCOMPARE( expectedValence(Element::Cl), 1 );
  
  QCOMPARE( expectedValence(Element::He), 0 );
  QCOMPARE( expectedValence(Element::Ne), 0 );
}

void ValenceTest::elementGroups()
{
  QCOMPARE( elementGroup(Element::H), 1 );
  QCOMPARE( elementGroup(Element::Li), 1 );
  QCOMPARE( elementGroup(Element::Na), 1 );
  QCOMPARE( elementGroup(Element::K), 1 );
  QCOMPARE( elementGroup(Element::Rb), 1 );
  QCOMPARE( elementGroup(Element::Cs), 1 );
  QCOMPARE( elementGroup(Element::Fr), 1 );

  QCOMPARE( elementGroup(Element::Be), 2 );
  QCOMPARE( elementGroup(Element::Mg), 2 );
  QCOMPARE( elementGroup(Element::Ca), 2 );
  QCOMPARE( elementGroup(Element::Sr), 2 );
  QCOMPARE( elementGroup(Element::Ba), 2 );
  QCOMPARE( elementGroup(Element::Ra), 2 );

  QCOMPARE( elementGroup(Element::B), 13 );
  QCOMPARE( elementGroup(Element::Al), 13 );
  QCOMPARE( elementGroup(Element::Ga), 13 );
  QCOMPARE( elementGroup(Element::In), 13 );
  QCOMPARE( elementGroup(Element::Tl), 13 );
  QCOMPARE( elementGroup(Element::Uut), 13 );

  QCOMPARE( elementGroup(Element::C), 14 );
  QCOMPARE( elementGroup(Element::Si), 14 );
  QCOMPARE( elementGroup(Element::Ge), 14 );
  QCOMPARE( elementGroup(Element::Sn), 14 );
  QCOMPARE( elementGroup(Element::Pb), 14 );

  QCOMPARE( elementGroup(Element::N), 15 );
  QCOMPARE( elementGroup(Element::P), 15 );
  QCOMPARE( elementGroup(Element::As), 15 );
  QCOMPARE( elementGroup(Element::Sb), 15 );
  QCOMPARE( elementGroup(Element::Bi), 15 );

  QCOMPARE( elementGroup(Element::O), 16 );
  QCOMPARE( elementGroup(Element::S), 16 );
  QCOMPARE( elementGroup(Element::Se), 16 );
  QCOMPARE( elementGroup(Element::Te), 16 );
  QCOMPARE( elementGroup(Element::Po), 16 );

  QCOMPARE( elementGroup(Element::F), 17 );
  QCOMPARE( elementGroup(Element::Cl), 17 );
  QCOMPARE( elementGroup(Element::Br), 17 );
  QCOMPARE( elementGroup(Element::I), 17 );
  QCOMPARE( elementGroup(Element::At), 17 );

  QCOMPARE( elementGroup(Element::He), 18 );
  QCOMPARE( elementGroup(Element::Ne), 18 );
  QCOMPARE( elementGroup(Element::Ar), 18 );
  QCOMPARE( elementGroup(Element::Kr), 18 );
  QCOMPARE( elementGroup(Element::Xe), 18 );
  QCOMPARE( elementGroup(Element::Rn), 18 );
}
    
void ValenceTest::valenceElectrons()
{
  QCOMPARE( numValenceElectrons(Element::H), 1 );
  QCOMPARE( numValenceElectrons(Element::K), 1 );
  
  QCOMPARE( numValenceElectrons(Element::Mg), 2 );
  QCOMPARE( numValenceElectrons(Element::Ca), 2 );
  
  QCOMPARE( numValenceElectrons(Element::B), 3 );
  QCOMPARE( numValenceElectrons(Element::Al), 3 );
  
  QCOMPARE( numValenceElectrons(Element::C), 4 );
  QCOMPARE( numValenceElectrons(Element::Si), 4 );

  QCOMPARE( numValenceElectrons(Element::N), 5 );
  QCOMPARE( numValenceElectrons(Element::P), 5 );
  
  QCOMPARE( numValenceElectrons(Element::O), 6 );
  QCOMPARE( numValenceElectrons(Element::S), 6 );
  
  QCOMPARE( numValenceElectrons(Element::F), 7 );
  QCOMPARE( numValenceElectrons(Element::Cl), 7 );
  
  QCOMPARE( numValenceElectrons(Element::He), 2 );
  QCOMPARE( numValenceElectrons(Element::Ne), 8 );


}

QTEST_MAIN(ValenceTest)

#include "moc_valencetest.cxx"
