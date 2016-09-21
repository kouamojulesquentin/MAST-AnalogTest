#include <iostream>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <stdexcept>

#include "SIT_reader.hpp"
#include "SystemModel.hpp"
#include "PrettyPrinter.hpp"

using namespace mast;


namespace
{
  //! Displays syntax on console
  void DisplaySyntax()
  {
    std::cout << "  Use -o for pipe from std::cin\n";
    std::cout << "  Just give a filename to parse from a file\n";
    std::cout << "  Use -h to get this menu\n";
  }
} // End of unnamed namespace


int main (const int argc, const char** argv)
{
  int retCode = EXIT_SUCCESS;

  try
  {
    /** check for the right # of arguments **/
    if (argc == 2)
    {
      auto sm = std::make_shared <mast::SystemModel>();
      SIT::SIT_Reader driver(sm);

      if (std::strncmp(argv[1], "-h", 2) == 0)  /** simple help menu **/
      {
        DisplaySyntax();
        return (EXIT_SUCCESS);
      }

      auto sourceDescription = "";
      if (std::strncmp(argv[1], "-o", 2) == 0) /** example for piping input from terminal, i.e., using cat **/
      {
        sourceDescription = "cin";
        driver.parse(std::cin);
      }
      else  /** example reading input from a file **/
      {
        sourceDescription = argv[1];
        driver.parse(argv[1]);  /** example reading input from a file **/
      }

      std::cout << "Parsing finished\n";

      if (driver.parsed_sut == nullptr)
      {
        std::cerr << "Failed to parse from: " << sourceDescription;
        return (EXIT_FAILURE);
      }

      auto gotPretty = PrettyPrinter::PrettyPrint(driver.parsed_sut, PrettyPrinterOptions::ShowNodeIsIgnored); // PrettyPrinterOptions::ShowSelectorProperties
      std::cout << gotPretty << "\n";
      auto result = sm->Check();
      std::cout << "Model Check returns " << result.MakeReport();
    }
    else
    {
      std::cerr << std::endl << "Missing command line parameter!" << std::endl;
      DisplaySyntax();
      return (EXIT_FAILURE);
    }
  }
  catch(std::invalid_argument& exc) { retCode = EXIT_FAILURE; std::cout << exc.what(); }
  catch(std::out_of_range&     exc) { retCode = EXIT_FAILURE; std::cout << exc.what(); }
  catch(std::logic_error&      exc) { retCode = EXIT_FAILURE; std::cout << exc.what(); }
  catch(std::runtime_error&    exc) { retCode = EXIT_FAILURE; std::cout << exc.what(); }
  catch(std::exception&        exc) { retCode = EXIT_FAILURE; std::cout << exc.what(); }
  catch(...)                        { retCode = EXIT_FAILURE; std::cout << "Got non std::exception"; }

  return (retCode);
}
