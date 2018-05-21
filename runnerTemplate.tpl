#include <cxxtest/XUnitPrinter.h>
#include <QApplication>


void messageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
  QByteArray localMsg = msg.toLocal8Bit();
  switch (type) {
    case QtDebugMsg:
      fprintf(stdout, "      Debug: (%s:%u, %s) %s\n", context.file, context.line, context.function, localMsg.constData());
      break;
    case QtWarningMsg:
      fprintf(stdout, "      Warning: (%s:%u, %s) %s\n", context.file, context.line, context.function, localMsg.constData());
      break;
    case QtCriticalMsg:
      fprintf(stdout, "      Critical: (%s:%u, %s) %s\n", context.file, context.line, context.function, localMsg.constData());
      break;
    case QtFatalMsg:
      fprintf(stderr, "      Fatal: (%s:%u, %s) %s\n", context.file, context.line, context.function, localMsg.constData());
      throw QString("Would have aborted here");
    case QtInfoMsg:
      fprintf(stdout, "      Info: (%s:%u, %s) %s\n", context.file, context.line, context.function, localMsg.constData());
  }
}

class MolsketchErrorPrinter : public CxxTest::ErrorPrinter {
public:
  void enterTest(const CxxTest::TestDescription &description) override {
    CxxTest::OutputStream::endl((*outputStream()) << "Starting test:  " << description.testName());
    ErrorPrinter::enterTest(description);
  }
  void leaveTest(const CxxTest::TestDescription &description) override {
    if (CxxTest::tracker().testSkipped())
      CxxTest::OutputStream::endl((*outputStream()) << "Skipped test:   " << description.testName());
    else if (!CxxTest::tracker().testFailed())
      CxxTest::OutputStream::endl((*outputStream()) << "Completed test: " << description.testName());
  }
  void trace(const char *file, int line, const char *message) override {
    (*outputStream()) << "  ";
    CxxTest::OutputStream::endl((*outputStream()) << "  " << file << ":" << line << ": Trace: " << message);
  }
  void enterSuite(const CxxTest::SuiteDescription &description) override {
    CxxTest::ErrorPrinter::enterSuite(description);
    CxxTest::OutputStream::endl(*outputStream());
  }
};

class MolsketchTestListener : public CxxTest::TeeListener {
public:
  CxxTest::XmlPrinter xml_printer;
  MolsketchErrorPrinter error_printer;

  MolsketchTestListener(CXXTEST_STD(ostream) &o = CXXTEST_STD(cout))
    : xml_printer(o)
  {
    setFirst(error_printer);
    setSecond(xml_printer);
  }

  int run() {
    CxxTest::TestRunner::runAllTests(*this);
    return CxxTest::tracker().failedTests();
  }
};

int main( int argc, char *argv[] ) {
  qInstallMessageHandler(messageOutput);
  char** argdummy = nullptr;
  int argcdummy = 0;
  QCoreApplication::setOrganizationName("SourceForge");
  QCoreApplication::setOrganizationDomain("sourceforge.net");
  QCoreApplication::setApplicationName("Molsetch");
  QApplication app(argcdummy, argdummy);

  std::ofstream ofstr("TEST-cxxtest.xml");
  MolsketchTestListener testPrinter(ofstr);
  CxxTest::RealWorldDescription::_worldName = "Molsketch";
  app.exit(CxxTest::Main< MolsketchTestListener >( testPrinter, argc, argv ));
}

<CxxTest world>
