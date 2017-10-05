<CxxTest preamble>

#include "g3log/g3log.hpp"
#include "g3log/logworker.hpp"
#include "LogFormatter.h"
#include "LoggerSinks.h"
#include "CustomFileSink.h"
#include <memory>
#include <string>

std::string Exe_Dir_Path; // Mainly for mast configuration related code

int main( int argc, char *argv[] )
{
    // ---------------- Set Exe_Dir_Path with only directory path of runner application
    //
    Exe_Dir_Path = std::string(argv[0]);
    auto sepPos  = Exe_Dir_Path.rfind("/"); // Search for last Linux directory separator
    if (sepPos == std::string::npos)
    {
      sepPos  = Exe_Dir_Path.rfind("\\");   // Search for last Windows directory separator
    }

    if (sepPos != std::string::npos)
    {
      Exe_Dir_Path.erase(sepPos);           // Remove exe name (keeping only directory path)
    }
    else
    {
      Exe_Dir_Path = ".";                   // When no separator ==> there is only application name ==> this is current directory
    }

    // ---------------- Initialize logger
    //
    auto logworker  = g3::LogWorker::createLogWorker();

    // ---------------- Sink for logging errors to std::cerr
    //
    auto cerrSink       = std::make_unique<g3::ErrorsOnCerrLoggerSink>(true, g3::LogFormatterUsage::Ignore);
    auto cerrSinkHandle = logworker->addSink(std::move(cerrSink), &g3::ErrorsOnCerrLoggerSink::ReceiveLogMessage);

    // ---------------- Sink for capturing all messages in memory
    //
    g3::UnitTestsLoggerSink::TSinkHandle sinkHandle = logworker->addSink(std::make_unique<g3::UnitTestsLoggerSink>(false), &g3::UnitTestsLoggerSink::ReceiveLogMessage);
    g3::UnitTestsLoggerSink::SetSinkHandle(sinkHandle);    // This is to allow g3log unit tests to have an access point to log at the end of the chain

    auto logFormatter = g3::LogFormatter();
    logFormatter.ShowDate(false);
    logFormatter.ShowTime(false);
    logFormatter.ShowFileName(false);
    logFormatter.ShowFunctionName(false);
    logFormatter.ShowLineNumber(false);


    auto customSink = std::make_unique<g3::CustomFileSink>("UT_Readers_Log.txt", g3::CustomFileSink::FlushMode::AutoBackground, logFormatter);
    customSink->Clear();

    auto customSinkHandle = logworker->addSink(std::move(customSink), &g3::CustomFileSink::ReceiveLogUnformattedMessage);


    g3::initializeLogging(logworker.get());
    LOG(INFO) << "Start of Unit tests";
    g3::logEnabled(false);

    // ---------------- Start UT framework
    //
    CxxTest::RealWorldDescription::_worldName = "Mast_Readers";

    CxxTest::ErrorPrinter tmp;

    int status = CxxTest::Main<CxxTest::ErrorPrinter>(tmp, argc, argv);

    g3::logEnabled(true);
    LOG(INFO) << "End of Unit tests";
    g3::logEnabled(false);
    return status;
}

<CxxTest world>
