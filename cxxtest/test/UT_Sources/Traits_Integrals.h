
#define CXXTEST_HAVE_STD
#include <cxxtest/TestSuite.h>

//
// This test suite tests CxxTest's conversion of different integral values to strings
//

class Traits_Integrals : public CxxTest::TestSuite
{
public:
    void testIntegerTraits()
    {
        TS_FAIL((unsigned char)      1);
        TS_FAIL((char)               0x0F);
        TS_FAIL((signed short int)  -12);
        TS_FAIL((unsigned short int) 34);
        TS_FAIL((signed int)        -123);
        TS_FAIL((unsigned int)       456);
        TS_FAIL((signed long int)   -12345);
        TS_FAIL((unsigned long int)  67890);
    }

    void testFloatTraits()
    {
        TS_FAIL(5.01234567890123f);
    }

    void testDoubleTrait()
    {
        TS_FAIL(5.012345678901234567890123);
    }

    void testBoolTraits()
    {
        TS_FAIL(true);
        TS_FAIL(false);
    }
};

//===========================================================================
// End of Traits_Integrals.h
//===========================================================================
