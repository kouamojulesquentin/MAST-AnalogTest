#include <cxxtest/TestSuite.h>

//
// This test suite demonstrates TS_ASSERT_SAME_FILES
//
#define SOURCE_DIR  "UT_Sources/"

class SameFiles : public CxxTest::TestSuite
{
public:

    void testAssertFiles()
    {
        TS_ASSERT_SAME_FILES(SOURCE_DIR "SameFiles.h", SOURCE_DIR "SameFiles.h");
    }

    void testAssertFileShorter()
    {
        TS_ASSERT_SAME_FILES(SOURCE_DIR "SameFiles.h", SOURCE_DIR "SameFilesLonger.h");
    }

    void testAssertFileLonger()
    {
        TS_ASSERT_SAME_FILES(SOURCE_DIR "SameFilesLonger.h", SOURCE_DIR "SameFiles.h");
    }

    void testAssertMessageSameFiles()
    {
        TSM_ASSERT_SAME_FILES("Not same files", SOURCE_DIR "SameFiles.h", SOURCE_DIR "SameData.h");
    }

    void testSafeAssertSameFiles()
    {
        ETS_ASSERT_SAME_FILES(SOURCE_DIR "SameFiles.h", SOURCE_DIR "SameFiles.h");
    }

    void testSafeAssertMessageSameFiles()
    {
        ETSM_ASSERT_SAME_FILES("Not same files", SOURCE_DIR "SameFiles.h", SOURCE_DIR "SameData.h");
    }
};

// This is a bit longer than SameFiles.h, so we can test the logic of file comparison
