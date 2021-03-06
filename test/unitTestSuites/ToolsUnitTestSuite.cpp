#include <cppunit/extensions/HelperMacros.h>


/** @brief A cppunit TestFixture to test the things in the "tools" directory.
 *
 * @author Mark Grimes (mark.grimes@bristol.ac.uk)
 * @date 03/Aug/2013
 */
class ToolsUnitTestSuite : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(ToolsUnitTestSuite);
	CPPUNIT_TEST(testLinearFitInputCheck);
	CPPUNIT_TEST(testLinearFitResult);
	CPPUNIT_TEST_SUITE_END();

protected:

public:
	void setUp();

protected:
	void testLinearFitInputCheck();
	void testLinearFitResult();
};





#include <cppunit/config/SourcePrefix.h>
#include <iostream>
#include <stdexcept>
#include "l1menu/tools/miscellaneous.h"

CPPUNIT_TEST_SUITE_REGISTRATION(ToolsUnitTestSuite);

void ToolsUnitTestSuite::setUp()
{

}

void ToolsUnitTestSuite::testLinearFitInputCheck()
{
	std::vector< std::pair<double,double> > coords;
	std::pair<double,double> slopeInterceptPair;

	// Make sure it fails if no parameters are provided
	CPPUNIT_ASSERT_THROW( slopeInterceptPair=l1menu::tools::simpleLinearFit( coords ), std::runtime_error );

	// Make sure it fails if only one input coordinate is provided
	coords.push_back( std::make_pair( 0.002, 78 ) );
	CPPUNIT_ASSERT_THROW( slopeInterceptPair=l1menu::tools::simpleLinearFit( coords ), std::runtime_error );
}

void ToolsUnitTestSuite::testLinearFitResult()
{
	std::vector< std::pair<double,double> > coords;
	double slope=1.34;
	double intercept=0.15;
	coords.push_back( std::make_pair( 3, 3*slope+intercept ) );
	coords.push_back( std::make_pair( 2.45, 2.45*slope+intercept ) );
	coords.push_back( std::make_pair( 23.23, 23.23*slope+intercept ) );
	coords.push_back( std::make_pair( 0.145, 0.145*slope+intercept ) );
	coords.push_back( std::make_pair( 0, 0*slope+intercept ) );
	coords.push_back( std::make_pair( -23, -23*slope+intercept ) );
	std::pair<double,double> slopeInterceptPair;
	CPPUNIT_ASSERT_NO_THROW( slopeInterceptPair=l1menu::tools::simpleLinearFit( coords ) );

	const double delta=0.000001;
	CPPUNIT_ASSERT_DOUBLES_EQUAL( slope, slopeInterceptPair.first, delta );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( intercept, slopeInterceptPair.second, delta );

	coords.clear();
	coords.push_back( std::make_pair( 0, 30966.3 ) );
	coords.push_back( std::make_pair( 0.9, 30966.3 ) );
	coords.push_back( std::make_pair( 1.8, 30966.3 ) );
	coords.push_back( std::make_pair( 2.7, 30966.3 ) );
	CPPUNIT_ASSERT_NO_THROW( slopeInterceptPair=l1menu::tools::simpleLinearFit( coords ) );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 0, slopeInterceptPair.first, delta );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 30966.3, slopeInterceptPair.second, delta );
}
