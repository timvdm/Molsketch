#include <QTest>
#include "coordinatestest.h"
#include "drawingtest.h"
#include "valencetest.h"

typedef QPair<QString, int> stringIntPair ;

stringIntPair runTest(QObject *testobject)
{
        return qMakePair(QString(testobject->metaObject()->className()),
                         QTest::qExec(testobject, QStringList())) ;
}

int main(int argc, char *argv[])
{
        QApplication app(argc, argv);
        QVector<stringIntPair> results ;
        results
            << runTest(new coordinatesTest())
            << runTest(new DrawingTest)
            << runTest(new ValenceTest)
               ;
        qDebug() << "=========== All tests done. ===========" ;
        foreach(const stringIntPair& result, results)
                qDebug() << (result.second ? "!!" : "  ")
                         << result.second
                         << result.first ;

        return 0 ;
}
