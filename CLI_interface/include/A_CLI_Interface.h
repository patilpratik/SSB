/*
 * A_CLI_Interface.h
 *
 *  Created on: 30-Jul-2017
 *      Author: pratik
 */

#ifndef A_CLI_INTERFACE_H_
#define A_CLI_INTERFACE_H_
#include <map>
namespace SSB
{
///
/// Print help
///
#define HELP 0X00000001
#define OPT_HELP "--help"

///
/// Print version information
///
#define VERSION 0x00000010
#define OPT_VER "--version"

///
/// \breif This is an abstract class that provides basic functionality of CLI interface
///

/// Options can be added or removed by inheriting this class and using provided methods
/// Inputs :
///         1) argc : number of total paramters
///         2) argv : memory address of the parameter list
///
///                The opts are
///                  1) help     : print help
///                  2) version  : display verison information

  class CliInterface
  {
  public:
    CliInterface ();

    ///
    /// Object construction with checking of the options
    ///
    CliInterface (int argc, char** argv);
    ///
    /// Returns true of opts are valid. false otherwise
    ///
    bool
    isValid ();
    ///
    /// Returns true if -h used
    ///
    bool
    help ();
    ///
    /// Returns true if -v used
    ///
    bool
    version ();	///
    /// Prints a help menu. Child can override this function
    /// \todo help menu yet to be printed
    virtual void
    printHelp () = 0;
    ///
    /// Prints version information
    ///
    virtual void
    printVersion () = 0;
    ///
    /// removes support for given option --------------------- move this to protected later
    ///
    bool
    remOptSupport (const std::string&);

    virtual
    ~CliInterface ();

  protected:
    ///
    /// checks short paramter, returns its code if unambiguos
    ///
    int
    checkArg (const std::string&);
    ///
    /// adds given option to the supported list
    /// Only derived class can use this method as the appropriate handling for the new option is also to be provided
    ///
    bool
    addOptSupport (const std::string&, const int&);
    ///
    /// Populates _supported_opt with default values
    ///
    virtual void
    initSupportedOpt ();
    ///
    /// each bit of this represents an option
    ///
    int _optList;
    ///
    /// It carries the list of supported paramters
    ///
    std::map<std::string, int> _supportedOpt;

    ///
    /// Set to true if all the parameters provided are valid
    ///
    bool _validationStatus;
  private:

  };
}
#endif /* A_CLI_INTERFACE_H_ */
