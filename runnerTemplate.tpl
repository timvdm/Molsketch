#include <cxxtest/XUnitPrinter.h>
#include <QApplication>

int main( int argc, char *argv[] ) {
    char** argdummy = nullptr;
    int argcdummy = 0;
    QCoreApplication::setOrganizationName("SourceForge");
    QCoreApplication::setOrganizationDomain("sourceforge.net");
    QCoreApplication::setApplicationName("Molsetch");
    QApplication app(argcdummy, argdummy);
    int status;
    std::ofstream ofstr("TEST-cxxtest.xml");
    CxxTest::XUnitPrinter tmp(ofstr);
    CxxTest::RealWorldDescription::_worldName = "cxxtest";
    status = CxxTest::Main< CxxTest::XUnitPrinter >( tmp, argc, argv );
    app.exit(status);
}

<CxxTest world>
