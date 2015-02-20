#include <QObject>
#include <QtTest>

#include <molscene.h>
#include <molview.h>
#include <actions/drawaction.h>
#include <QVector>

Q_DECLARE_METATYPE(QVector<QLineF>)


using namespace Molsketch;

class DrawingTest : public QObject
{
  Q_OBJECT
public:
  explicit DrawingTest(QObject *parent = 0);
  ~DrawingTest();
private slots:
  void initTestCase();
  void cleanupTestCase();
  void init();
  void cleanup();

  void testDrawing();
  void testDrawing_data();

private:
  Molsketch::MolScene *scene;
  Molsketch::drawAction *action;
  Molsketch::MolView *view;
};

DrawingTest::DrawingTest(QObject *parent) : QObject(parent)
{
}

DrawingTest::~DrawingTest()
{

}

void DrawingTest::initTestCase()
{
}

void DrawingTest::cleanupTestCase()
{
}

void DrawingTest::init()
{
  scene = new MolScene;
  view = new MolView(scene);
  action = new drawAction(scene);
  action->setChecked(true);
}

void DrawingTest::cleanup()
{
  delete view;
  delete scene;
}

void DrawingTest::testDrawing()
{
  QFETCH(QVector<QLineF>, lines);

  foreach(const QLineF& line, lines)
  {
    QTest::mousePress(view, Qt::LeftButton, 0,
                      view->mapFromScene(line.p1()));
    QTest::mouseRelease(view, Qt::LeftButton, 0,
                        view->mapFromScene(line.p2()));
  }

  // now do the checking...
}

void DrawingTest::testDrawing_data()
{
  QTest::addColumn<QVector<QLineF> >("lines");

  QTest::newRow("single atom") << (QVector<QLineF>()
                                   << QLineF(0,0,0,0)
                                   );
}


QTEST_MAIN(DrawingTest)

#if QT_VERSION < 0x050000
#ifdef QMAKEBUILD
#include "drawingtest.moc"
#else
#include "moc_drawingtest.cxx"
#endif
#else
#include "drawingtest.moc"
#endif
