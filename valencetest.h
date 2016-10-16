#include <cxxtest/TestSuite.h>
#include <atom.h>
#include <element.h>

using namespace Molsketch;

struct ElementData
{
  QString symbol;
  Element::Elements element;
  int expectedValences, elementGroup, valenceElectrons, implicitHydrogens, charge;
  ElementData(const QString& symbol,
              Element::Elements element,
              int expectedValences,
              int elementGroup,
              int valenceElectrons,
              int implicitHydrogens,
              int charge)
    : symbol(symbol),
      element(element),
      expectedValences(expectedValences),
      elementGroup(elementGroup),
      valenceElectrons(valenceElectrons),
      implicitHydrogens(implicitHydrogens),
      charge(charge)
  {}
  ElementData()
    : symbol(""),
      element(Element::Dummy),
      expectedValences(-10),
      elementGroup(-10),
      valenceElectrons(-10),
      implicitHydrogens(-10),
      charge(-10)
  {}
};

QVector<ElementData> groupData(const QVector<QPair<QString, Element::Elements> >& elements,
                               int expectedValences,
                               int elementGroup,
                               int valenceElectrons,
                               int implicitHydrogens,
                               int charge)
{
  QVector<ElementData> result;
  for(auto element : elements)
    result << ElementData(element.first,
                          element.second,
                          expectedValences,
                          elementGroup,
                          valenceElectrons,
                          implicitHydrogens,
                          charge);
  return result;
}

QVector<ElementData> testData() {
  using namespace Element;
  typedef ElementData el;
  typedef QPair<QString, Element::Elements> ep;
  typedef QVector<ep> epv;
  return QVector<el>()
      << groupData(epv()
                   << ep("H", H)
                   << ep("Li", Li)
                   << ep("Na", Na)
                   << ep("K", K)
                   << ep("Rb", Rb)
                   << ep("Cs", Cs)
                   << ep("Fr", Fr), 1,1,1,0,1)
      << el("He", He, 0, 18, 2, 0, 0)
      << groupData(epv()
                   << ep("Ne", Ne)
                   << ep("Ar", Ar)
                   << ep("Kr", Kr)
                   << ep("Xe", Xe)
                   << ep("Rn", Rn), 0, 18, 8, 0, 0)
      << groupData(epv()
                   << ep("Be", Be)
                   << ep("Mg", Mg)
                   << ep("Ca", Ca)
                   << ep("Sr", Sr)
                   << ep("Ba", Ba)
                   << ep("Ra", Ra), 2, 2, 2, 0, 2)
      << groupData(epv()
                   << ep("B", B)
                   << ep("Al", Al)
                   << ep("Ga", Ga)
                   << ep("In", In)
                   << ep("Tl", Tl), 3, 13, 3, 0, 3)
      << groupData(epv()
                   << ep("C", C)
                   << ep("Si", Si)
                   << ep("Ge", Ge)
                   << ep("Sn", Sn)
                   << ep("Pb", Pb), 4, 14, 4, 0, 4)
      << groupData(epv()
                   << ep("N", N)
                   << ep("P", P)
                   << ep("As", As)
                   << ep("Sb", Sb)
                   << ep("Bi", Bi), 3, 15, 5, 0, 0) // TODO expected charge is weird!
      << groupData(epv()
                   << ep("O", O)
                   << ep("S", S)
                   << ep("Se", Se)
                   << ep("Te", Te)
                   << ep("Po", Po), 2, 16, 6, 0, 0)
      << groupData(epv()
                   << ep("F", F)
                   << ep("Cl", Cl)
                   << ep("Br", Br)
                   << ep("I", I)
                   << ep("At", At), 1, 17, 7, 0, -1)
         // TODO transition metals
         // TODO check availability of heaviest group elements
         ;
}

const char* convert(const QString& string)
{
  return string.toStdString().c_str();
}

class ValenceTest : public CxxTest::TestSuite
{
public:
  void testExpectedValences()
  {
    Atom atom(QPointF(0,0), "R", true);
    for(ElementData d : testData())
    {
      TSM_ASSERT_EQUALS(convert(d.symbol + " expected valence"), d.expectedValences, expectedValence(d.element));
      TSM_ASSERT_EQUALS(convert(d.symbol + " group"), d.elementGroup, elementGroup(d.element));
      TSM_ASSERT_EQUALS(convert(d.symbol + " valence electrons"), d.valenceElectrons, numValenceElectrons(d.element));
      TSM_ASSERT_EQUALS(convert(d.symbol + " implicit hydrogens"), d.implicitHydrogens, atom.numImplicitHydrogens());
      atom.setElement(d.symbol);
      TSM_ASSERT_EQUALS(convert(d.symbol + " charge"), d.charge, atom.charge());
    }
  }
};
