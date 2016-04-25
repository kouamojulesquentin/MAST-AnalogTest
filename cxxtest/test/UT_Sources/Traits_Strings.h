
#define CXXTEST_HAVE_STD
#include <cxxtest/TestSuite.h>

//
// This test suite tests CxxTest's conversion of different values to strings
//

class Traits_Strings : public CxxTest::TestSuite
{
public:
    void testCharTraits()
    {
        TS_FAIL('A');
        TS_FAIL('\x04');
        TS_FAIL('\x1B');
        TS_FAIL('\0');
        TS_FAIL('\r');
        TS_FAIL('\n');
        TS_FAIL('\b');
        TS_FAIL('\t');
        TS_FAIL('\a');
        TS_FAIL((char) - 5);
    }

    void testStringTraits()
    {
        TS_FAIL("(char *) is displayed as-is\n");
    }

    void testStdStringTraits()
    {
        CxxTest::setAbortTestOnFail(false);
        typedef CXXTEST_STD(string) String;
        TS_FAIL(String("std::string is displayed with \"\""));
        TS_FAIL(String("Escapes\rAre\rTranslated"));
        TS_FAIL(String("As are unprintable chars: \x12\x34\x56\x78"));
    }
};
