<CxxTest preamble>

#include <memory>

int main( int argc, char *argv[] )
{

    // ---------------- Start UT framework
    //
    CxxTest::RealWorldDescription::_worldName = "External_Lib";

    CxxTest::ErrorPrinter tmp;

    int status = CxxTest::Main<CxxTest::ErrorPrinter>(tmp, argc, argv);

    return status;
}

<CxxTest world>
