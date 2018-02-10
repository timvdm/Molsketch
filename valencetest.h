/***************************************************************************
 *   Copyright (C) 2016 by Hendrik Vennekate                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/
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
                   << ep("Tl", Tl)
                   << ep("Uut", Uut), 3, 13, 3, 0, 3)
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
      << groupData(epv()
                   << ep("Sc", Sc)
                   << ep("Y", Y)
                   << ep("La", La)
                   << ep("Ac", Ac), 0, 3, 1, 0, 1)
      << groupData(epv()
                   << ep("Ti", Ti)
                   << ep("Zr", Zr)
                   << ep("Hf", Hf)
                   << ep("Rf", Rf), 0, 4, 2, 0, 2)
      << groupData(epv()
                   << ep("V", V)
                   << ep("Nb", Nb)
                   << ep("Ta", Ta)
                   << ep("Db", Db), 0, 5, 3, 0, 3)
      << groupData(epv()
                   << ep("Cr", Cr)
                   << ep("Mo", Mo)
                   << ep("W", W)
                   << ep("Sg", Sg), 0, 6, 4, 0, 4)
      << groupData(epv()
                   << ep("Mn", Mn)
                   << ep("Tc", Tc)
                   << ep("Re", Re)
                   << ep("Bh", Bh), 0, 7, 5, 0, 5)
      << groupData(epv()
                   << ep("Fe", Fe)
                   << ep("Ru", Ru)
                   << ep("Os", Os)
                   << ep("Hs", Hs), 0, 8, 6, 0, 6)
      << groupData(epv()
                   << ep("Co", Co)
                   << ep("Rh", Rh)
                   << ep("Ir", Ir)
                   << ep("Mt", Mt), 0, 9, 7, 0, 7)
      << groupData(epv()
                   << ep("Ni", Ni)
                   << ep("Pd", Pd)
                   << ep("Pt", Pt)
                   << ep("Ds", Ds), 0, 10, 8, 0, 8)
      << groupData(epv()
                   << ep("Cu", Cu)
                   << ep("Ag", Ag)
                   << ep("Au", Au)
                   << ep("Rg", Rg), 0, 11, 9, 0, 9)
      << groupData(epv()
                   << ep("Zn", Zn)
                   << ep("Cd", Cd)
                   << ep("Hg", Hg)
                   << ep("Uub", Uub), 0, 12, 10, 0, 10)
      << groupData(epv()
                   << ep("Ce", Ce)
                   << ep("Th", Th)
                   << ep("Pr", Pr)
                   << ep("Pa", Pa)
                   << ep("Nd", Nd)
                   << ep("U", U)
                   << ep("Pm", Pm)
                   << ep("Np", Np)
                   << ep("Sm", Sm)
                   << ep("Pu", Pu)
                   << ep("Eu", Eu)
                   << ep("Am", Am)
                   << ep("Gd", Gd)
                   << ep("Cm", Cm)
                   << ep("Tb", Tb)
                   << ep("Bk", Bk)
                   << ep("Dy", Dy)
                   << ep("Cf", Cf)
                   << ep("Ho", Ho)
                   << ep("Es", Es)
                   << ep("Er", Er)
                   << ep("Fm", Fm)
                   << ep("Tm", Tm)
                   << ep("Md", Md)
                   << ep("Yb", Yb)
                   << ep("No", No)
                   << ep("Lu", Lu)
                   << ep("Lr", Lr), 0, 3, 1, 0, 1);
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
