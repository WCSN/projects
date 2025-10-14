#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <iostream>
#include <stdexcept>
#include <cppunit/XmlOutputter.h>
#include <mt/logger/LoggerFactory.h>
#include <mt/logger/Logger.h>

int main( int argc, char* argv[] )
{
	mt::LoggerFactory::instance()->init("0.0.0.0", 0, "DEBUG", false);
	CPPUNIT_NS::stdCOut() << "Testing mtlib\n";
  // Retreive test path from command line first argument. Default to "" which resolve
  // to the top level suite.
  std::string testPath = (argc > 1) ? std::string(argv[1]) : std::string("");

  // Create the event manager and test controller
  CPPUNIT_NS::TestResult controller;

  // Add a listener that colllects test result
  CPPUNIT_NS::TestResultCollector result;
  controller.addListener( &result );

  // Add a listener that print dots as test run.
  CPPUNIT_NS::BriefTestProgressListener progress;
  controller.addListener( &progress );

  // Add the top suite to the test runner
  CPPUNIT_NS::TestRunner runner;
  runner.addTest( CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest() );
  try
  {
		if(!testPath.empty())
			CPPUNIT_NS::stdCOut() << "Running "  <<  testPath << "\n";

    runner.run( controller, testPath );

    CPPUNIT_NS::stdCOut() << "\n";

    // Print test in a compiler compatible format.
    CPPUNIT_NS::CompilerOutputter outputter( &result, CPPUNIT_NS::stdCOut() );
    outputter.write();

// Uncomment this for XML output
   // std::ofstream file( "tests.xml" );
   // CPPUNIT_NS::XmlOutputter xml( &result, file );
   // xml.setStyleSheet( "report.xsl" );
   // xml.write();
   // file.close();
  }
  catch ( std::invalid_argument &e )  // Test path not resolved
  {
    CPPUNIT_NS::stdCOut()  <<  "\n"
                            <<  "ERROR: "  <<  e.what()
                            << "\n";
    return 0;
  }

  return result.wasSuccessful() ? 0 : 1;
}
