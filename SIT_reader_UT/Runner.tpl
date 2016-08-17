<CxxTest preamble>

#include "g3log/g3log.hpp"
#include "g3log/logworker.hpp"
#include "LogFormatter.h"
#include "LoggerSinks.h"
#include "CustomFileSink.h"
#include <memory>

int main( int argc, char *argv[] )
{
    // ---------------- Initialize logger
    //
    auto logworker  = g3::LogWorker::createLogWorker();

    g3::UnitTestsLoggerSink::TSinkHandle sinkHandle = logworker->addSink(std::make_unique<g3::UnitTestsLoggerSink>(false), &g3::UnitTestsLoggerSink::ReceiveLogMessage);
    g3::UnitTestsLoggerSink::SetSinkHandle(sinkHandle);    // This is to allow g3log unit tests to have an access point to log at the end of the chain

    auto logFormatter = g3::LogFormatter();
    logFormatter.ShowDate(false);
    logFormatter.ShowTime(false);
    logFormatter.ShowFileName(false);
    logFormatter.ShowFunctionName(false);
    logFormatter.ShowLineNumber(false);

    auto customSink = std::make_unique<g3::CustomFileSink>("Log.txt", g3::CustomFileSink::FlushMode::AutoBackground, logFormatter);
    customSink->Clear();

    auto customSinkHandle = logworker->addSink(std::move(customSink), &g3::CustomFileSink::ReceiveLogUnformattedMessage);

    g3::initializeLogging(logworker.get());
    LOG(INFO) << "Start of Unit tests";
    g3::logEnabled(false);

    // ---------------- Start UT framework
    //
    CxxTest::RealWorldDescription::_worldName = "Mast_SIT_reader";

    CxxTest::ErrorPrinter tmp;

    int status = CxxTest::Main<CxxTest::ErrorPrinter>(tmp, argc, argv);

    g3::logEnabled(true);
    LOG(INFO) << "End of Unit tests";
    g3::logEnabled(false);
    return status;
}

<CxxTest world>
