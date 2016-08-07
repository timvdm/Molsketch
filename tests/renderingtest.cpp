#include "renderingtest.h"
#include "molscene.h"

#include <molecule.h>

using namespace  Molsketch;

RenderingTest::RenderingTest(QObject *parent) :
  QObject(parent)
{
}

RenderingTest::~RenderingTest()
{

}

void RenderingTest::initTestCase()
{

}

void RenderingTest::cleanupTestCase()
{

}

void RenderingTest::init()
{
  scene = new MolScene;
}

void RenderingTest::cleanup()
{
  delete scene;
}

void RenderingTest::testRenderBonds()
{
  QFETCH(QPointF, coordinateA);
  QFETCH(QPointF, coordinateB);
  QFETCH(QString, symbolA);
  QFETCH(QString, symbolB);
  QFETCH(QString, expectedSvg);

  Atom *atomA = new Atom(coordinateA, symbolA), *atomB = new Atom(coordinateB, symbolB);
  Bond *bond = new Bond(atomA, atomB);
  Molecule *molecule = new Molecule(QSet<Atom*>() << atomA << atomB, QSet<Bond*>() << bond);
  scene->addItem(molecule);
  QByteArray svg = scene->toSvg();
//  qDebug() << svg.toBase64();
//  qDebug() << svg;
  if (svg != expectedSvg) qDebug() << svg;
  Q_ASSERT(svg == expectedSvg);
}

void RenderingTest::testRenderBonds_data()
{
  QTest::addColumn<QPointF>("coordinateA");
  QTest::addColumn<QPointF>("coordinateB");
  QTest::addColumn<QString>("symbolA");
  QTest::addColumn<QString>("symbolB");
  QTest::addColumn<QString>("expectedSvg");

  QTest::newRow("Standard horizontal") << QPointF(0,0)
                            << QPointF(40,0)
                            << "A"
                            << "A"
                            << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n<svg width=\"17.6389mm\" height=\"6.35mm\"\n viewBox=\"-5 -9 50 18\"\n xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\"  version=\"1.2\" baseProfile=\"tiny\">\n<title>MolsKetch Drawing</title>\n<desc>Generated with Qt</desc>\n<defs>\n</defs>\n<g fill=\"none\" stroke=\"black\" stroke-width=\"1\" fill-rule=\"evenodd\" stroke-linecap=\"square\" stroke-linejoin=\"bevel\" >\n\n<g fill=\"none\" stroke=\"#000000\" stroke-opacity=\"1\" stroke-width=\"1\" stroke-linecap=\"square\" stroke-linejoin=\"bevel\" transform=\"matrix(1,0,0,1,0,0)\"\nfont-family=\"Noto Sans\" font-size=\"10\" font-weight=\"400\" font-style=\"normal\" \n>\n</g>\n\n<g fill=\"none\" stroke=\"#000000\" stroke-opacity=\"1\" stroke-width=\"1\" stroke-linecap=\"square\" stroke-linejoin=\"bevel\" transform=\"matrix(1,0,0,1,0,0)\"\nfont-family=\"Noto Sans\" font-size=\"10\" font-weight=\"400\" font-style=\"normal\" \n>\n</g>\n\n<g fill=\"none\" stroke=\"#000000\" stroke-opacity=\"1\" stroke-width=\"1\" stroke-linecap=\"square\" stroke-linejoin=\"bevel\" transform=\"matrix(1,0,0,1,0,0)\"\nfont-family=\"Noto Sans\" font-size=\"10\" font-weight=\"400\" font-style=\"normal\" \n>\n</g>\n\n<g fill=\"none\" stroke=\"#000000\" stroke-opacity=\"1\" stroke-width=\"1\" stroke-linecap=\"square\" stroke-linejoin=\"bevel\" transform=\"matrix(1,0,0,1,0,0)\"\nfont-family=\"Noto Sans\" font-size=\"10\" font-weight=\"400\" font-style=\"normal\" \n>\n</g>\n\n<g fill=\"none\" stroke=\"#000000\" stroke-opacity=\"1\" stroke-width=\"2\" stroke-linecap=\"round\" stroke-linejoin=\"bevel\" transform=\"matrix(1,0,0,1,0,0)\"\nfont-family=\"Noto Sans\" font-size=\"10\" font-weight=\"400\" font-style=\"normal\" \n>\n<polyline fill=\"none\" vector-effect=\"none\" points=\"32,0 8,0 \" />\n</g>\n\n<g fill=\"none\" stroke=\"#000000\" stroke-opacity=\"1\" stroke-width=\"2\" stroke-linecap=\"round\" stroke-linejoin=\"bevel\" transform=\"matrix(1,0,0,1,0,0)\"\nfont-family=\"Noto Sans\" font-size=\"10\" font-weight=\"400\" font-style=\"normal\" \n>\n</g>\n\n<g fill=\"none\" stroke=\"#000000\" stroke-opacity=\"1\" stroke-width=\"1\" stroke-linecap=\"square\" stroke-linejoin=\"bevel\" transform=\"matrix(1,0,0,1,0,0)\"\nfont-family=\"Noto Sans\" font-size=\"10\" font-weight=\"400\" font-style=\"normal\" \n>\n</g>\n\n<g fill=\"none\" stroke=\"#000000\" stroke-opacity=\"1\" stroke-width=\"1\" stroke-linecap=\"square\" stroke-linejoin=\"bevel\" transform=\"matrix(1,0,0,1,0,0)\"\nfont-family=\"Noto Sans\" font-size=\"10\" font-weight=\"400\" font-style=\"normal\" \n>\n</g>\n\n<g fill=\"none\" stroke=\"#000000\" stroke-opacity=\"1\" stroke-width=\"1\" stroke-linecap=\"square\" stroke-linejoin=\"bevel\" transform=\"matrix(1,0,0,1,40,0)\"\nfont-family=\"Noto Sans\" font-size=\"10\" font-weight=\"400\" font-style=\"normal\" \n>\n</g>\n\n<g fill=\"none\" stroke=\"#000000\" stroke-opacity=\"1\" stroke-width=\"1\" stroke-linecap=\"square\" stroke-linejoin=\"bevel\" transform=\"matrix(1,0,0,1,40,0)\"\nfont-family=\"Noto Sans\" font-size=\"10\" font-weight=\"400\" font-style=\"normal\" \n>\n<text fill=\"#000000\" fill-opacity=\"1\" stroke=\"none\" xml:space=\"preserve\" x=\"-4\" y=\"5\" font-family=\"Noto Sans\" font-size=\"10\" font-weight=\"400\" font-style=\"normal\" \n >A</text>\n</g>\n\n<g fill=\"none\" stroke=\"#000000\" stroke-opacity=\"1\" stroke-width=\"1\" stroke-linecap=\"square\" stroke-linejoin=\"bevel\" transform=\"matrix(1,0,0,1,40,0)\"\nfont-family=\"Noto Sans\" font-size=\"10\" font-weight=\"400\" font-style=\"normal\" \n>\n</g>\n\n<g fill=\"none\" stroke=\"#000000\" stroke-opacity=\"1\" stroke-width=\"1\" stroke-linecap=\"square\" stroke-linejoin=\"bevel\" transform=\"matrix(1,0,0,1,40,0)\"\nfont-family=\"Noto Sans\" font-size=\"10\" font-weight=\"400\" font-style=\"normal\" \n>\n</g>\n\n<g fill=\"none\" stroke=\"#000000\" stroke-opacity=\"1\" stroke-width=\"1\" stroke-linecap=\"square\" stroke-linejoin=\"bevel\" transform=\"matrix(1,0,0,1,0,0)\"\nfont-family=\"Noto Sans\" font-size=\"10\" font-weight=\"400\" font-style=\"normal\" \n>\n</g>\n\n<g fill=\"none\" stroke=\"#000000\" stroke-opacity=\"1\" stroke-width=\"1\" stroke-linecap=\"square\" stroke-linejoin=\"bevel\" transform=\"matrix(1,0,0,1,0,0)\"\nfont-family=\"Noto Sans\" font-size=\"10\" font-weight=\"400\" font-style=\"normal\" \n>\n<text fill=\"#000000\" fill-opacity=\"1\" stroke=\"none\" xml:space=\"preserve\" x=\"-4\" y=\"5\" font-family=\"Noto Sans\" font-size=\"10\" font-weight=\"400\" font-style=\"normal\" \n >A</text>\n</g>\n\n<g fill=\"none\" stroke=\"#000000\" stroke-opacity=\"1\" stroke-width=\"1\" stroke-linecap=\"square\" stroke-linejoin=\"bevel\" transform=\"matrix(1,0,0,1,0,0)\"\nfont-family=\"Noto Sans\" font-size=\"10\" font-weight=\"400\" font-style=\"normal\" \n>\n</g>\n\n<g fill=\"none\" stroke=\"#000000\" stroke-opacity=\"1\" stroke-width=\"1\" stroke-linecap=\"square\" stroke-linejoin=\"bevel\" transform=\"matrix(1,0,0,1,0,0)\"\nfont-family=\"Noto Sans\" font-size=\"10\" font-weight=\"400\" font-style=\"normal\" \n>\n</g>\n\n<g fill=\"none\" stroke=\"#000000\" stroke-opacity=\"1\" stroke-width=\"1\" stroke-linecap=\"square\" stroke-linejoin=\"bevel\" transform=\"matrix(1,0,0,1,0,0)\"\nfont-family=\"Noto Sans\" font-size=\"10\" font-weight=\"400\" font-style=\"normal\" \n>\n</g>\n</g>\n</svg>\n";

  QTest::newRow("Short horizontal") << QPointF(0,0)
                            << QPointF(25,0)
                            << "A"
                            << "A"
                            << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n<svg width=\"17.6389mm\" height=\"6.35mm\"\n viewBox=\"-5 -9 50 18\"\n xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\"  version=\"1.2\" baseProfile=\"tiny\">\n<title>MolsKetch Drawing</title>\n<desc>Generated with Qt</desc>\n<defs>\n</defs>\n<g fill=\"none\" stroke=\"black\" stroke-width=\"1\" fill-rule=\"evenodd\" stroke-linecap=\"square\" stroke-linejoin=\"bevel\" >\n\n<g fill=\"none\" stroke=\"#000000\" stroke-opacity=\"1\" stroke-width=\"1\" stroke-linecap=\"square\" stroke-linejoin=\"bevel\" transform=\"matrix(1,0,0,1,0,0)\"\nfont-family=\"Noto Sans\" font-size=\"10\" font-weight=\"400\" font-style=\"normal\" \n>\n</g>\n\n<g fill=\"none\" stroke=\"#000000\" stroke-opacity=\"1\" stroke-width=\"1\" stroke-linecap=\"square\" stroke-linejoin=\"bevel\" transform=\"matrix(1,0,0,1,0,0)\"\nfont-family=\"Noto Sans\" font-size=\"10\" font-weight=\"400\" font-style=\"normal\" \n>\n</g>\n\n<g fill=\"none\" stroke=\"#000000\" stroke-opacity=\"1\" stroke-width=\"1\" stroke-linecap=\"square\" stroke-linejoin=\"bevel\" transform=\"matrix(1,0,0,1,0,0)\"\nfont-family=\"Noto Sans\" font-size=\"10\" font-weight=\"400\" font-style=\"normal\" \n>\n</g>\n\n<g fill=\"none\" stroke=\"#000000\" stroke-opacity=\"1\" stroke-width=\"1\" stroke-linecap=\"square\" stroke-linejoin=\"bevel\" transform=\"matrix(1,0,0,1,0,0)\"\nfont-family=\"Noto Sans\" font-size=\"10\" font-weight=\"400\" font-style=\"normal\" \n>\n</g>\n\n<g fill=\"none\" stroke=\"#000000\" stroke-opacity=\"1\" stroke-width=\"2\" stroke-linecap=\"round\" stroke-linejoin=\"bevel\" transform=\"matrix(1,0,0,1,0,0)\"\nfont-family=\"Noto Sans\" font-size=\"10\" font-weight=\"400\" font-style=\"normal\" \n>\n<polyline fill=\"none\" vector-effect=\"none\" points=\"32,0 8,0 \" />\n</g>\n\n<g fill=\"none\" stroke=\"#000000\" stroke-opacity=\"1\" stroke-width=\"2\" stroke-linecap=\"round\" stroke-linejoin=\"bevel\" transform=\"matrix(1,0,0,1,0,0)\"\nfont-family=\"Noto Sans\" font-size=\"10\" font-weight=\"400\" font-style=\"normal\" \n>\n</g>\n\n<g fill=\"none\" stroke=\"#000000\" stroke-opacity=\"1\" stroke-width=\"1\" stroke-linecap=\"square\" stroke-linejoin=\"bevel\" transform=\"matrix(1,0,0,1,0,0)\"\nfont-family=\"Noto Sans\" font-size=\"10\" font-weight=\"400\" font-style=\"normal\" \n>\n</g>\n\n<g fill=\"none\" stroke=\"#000000\" stroke-opacity=\"1\" stroke-width=\"1\" stroke-linecap=\"square\" stroke-linejoin=\"bevel\" transform=\"matrix(1,0,0,1,0,0)\"\nfont-family=\"Noto Sans\" font-size=\"10\" font-weight=\"400\" font-style=\"normal\" \n>\n</g>\n\n<g fill=\"none\" stroke=\"#000000\" stroke-opacity=\"1\" stroke-width=\"1\" stroke-linecap=\"square\" stroke-linejoin=\"bevel\" transform=\"matrix(1,0,0,1,40,0)\"\nfont-family=\"Noto Sans\" font-size=\"10\" font-weight=\"400\" font-style=\"normal\" \n>\n</g>\n\n<g fill=\"none\" stroke=\"#000000\" stroke-opacity=\"1\" stroke-width=\"1\" stroke-linecap=\"square\" stroke-linejoin=\"bevel\" transform=\"matrix(1,0,0,1,40,0)\"\nfont-family=\"Noto Sans\" font-size=\"10\" font-weight=\"400\" font-style=\"normal\" \n>\n<text fill=\"#000000\" fill-opacity=\"1\" stroke=\"none\" xml:space=\"preserve\" x=\"-4\" y=\"5\" font-family=\"Noto Sans\" font-size=\"10\" font-weight=\"400\" font-style=\"normal\" \n >A</text>\n</g>\n\n<g fill=\"none\" stroke=\"#000000\" stroke-opacity=\"1\" stroke-width=\"1\" stroke-linecap=\"square\" stroke-linejoin=\"bevel\" transform=\"matrix(1,0,0,1,40,0)\"\nfont-family=\"Noto Sans\" font-size=\"10\" font-weight=\"400\" font-style=\"normal\" \n>\n</g>\n\n<g fill=\"none\" stroke=\"#000000\" stroke-opacity=\"1\" stroke-width=\"1\" stroke-linecap=\"square\" stroke-linejoin=\"bevel\" transform=\"matrix(1,0,0,1,40,0)\"\nfont-family=\"Noto Sans\" font-size=\"10\" font-weight=\"400\" font-style=\"normal\" \n>\n</g>\n\n<g fill=\"none\" stroke=\"#000000\" stroke-opacity=\"1\" stroke-width=\"1\" stroke-linecap=\"square\" stroke-linejoin=\"bevel\" transform=\"matrix(1,0,0,1,0,0)\"\nfont-family=\"Noto Sans\" font-size=\"10\" font-weight=\"400\" font-style=\"normal\" \n>\n</g>\n\n<g fill=\"none\" stroke=\"#000000\" stroke-opacity=\"1\" stroke-width=\"1\" stroke-linecap=\"square\" stroke-linejoin=\"bevel\" transform=\"matrix(1,0,0,1,0,0)\"\nfont-family=\"Noto Sans\" font-size=\"10\" font-weight=\"400\" font-style=\"normal\" \n>\n<text fill=\"#000000\" fill-opacity=\"1\" stroke=\"none\" xml:space=\"preserve\" x=\"-4\" y=\"5\" font-family=\"Noto Sans\" font-size=\"10\" font-weight=\"400\" font-style=\"normal\" \n >A</text>\n</g>\n\n<g fill=\"none\" stroke=\"#000000\" stroke-opacity=\"1\" stroke-width=\"1\" stroke-linecap=\"square\" stroke-linejoin=\"bevel\" transform=\"matrix(1,0,0,1,0,0)\"\nfont-family=\"Noto Sans\" font-size=\"10\" font-weight=\"400\" font-style=\"normal\" \n>\n</g>\n\n<g fill=\"none\" stroke=\"#000000\" stroke-opacity=\"1\" stroke-width=\"1\" stroke-linecap=\"square\" stroke-linejoin=\"bevel\" transform=\"matrix(1,0,0,1,0,0)\"\nfont-family=\"Noto Sans\" font-size=\"10\" font-weight=\"400\" font-style=\"normal\" \n>\n</g>\n\n<g fill=\"none\" stroke=\"#000000\" stroke-opacity=\"1\" stroke-width=\"1\" stroke-linecap=\"square\" stroke-linejoin=\"bevel\" transform=\"matrix(1,0,0,1,0,0)\"\nfont-family=\"Noto Sans\" font-size=\"10\" font-weight=\"400\" font-style=\"normal\" \n>\n</g>\n</g>\n</svg>\n";

//  QTest::newRow("Long horizontal") << QPointF(0,0)
//                            << QPointF(60,0)
//                            << "A"
//                            << "A"
//                            << "";
//  QTest::newRow("Extra long horizontal") << QPointF(0,0)
//                            << QPointF(80,0)
//                            << "A"
//                            << "A"
//                            << "";
}
