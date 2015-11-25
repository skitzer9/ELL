// CommandLineParser.h

// cjacobs: Next steps to do:
// * Add policy field for options (required, set only once, set at least once, last wins, collect all, ...)
// * Add more flexible constraints (using a callback?) for non-enum parameters
// * have one "master" parser that has subordinate parsers to handle parameters for different modules (e.g., tree learner vs. booster vs. global optimizer)
// * have conditional parsers whose set of known options depends on some other option --- maybe via polymorphism (so, have a gradient_descent parser and 
//   a sdca parser, both of which are subclasses of a global_descent parser, the choice of which to use is conditional on the gd_alg option
// * tokenize the input stream based on whitespace, then split on entries starting with '-' --- the first item will be the option, followed by zero or more values
//   in this case, we maybe need to use the special '--' option to specify positional / unnamed arguments (and/or have them appear first)

#pragma once

#include <iostream>
using std::ostream;

#include <vector>
using std::vector;

#include <map>
using std::map;

#include <set>
using std::set;

#include <functional>
using std::function;

#include <string>
using std::string;
using std::pair;

namespace utilities
{
    class CommandLineParser;
    class ParsedArgSet
    {
    public:
        virtual void AddArgs(CommandLineParser& parser) {};
    };

    /// TODO: document
    ///
    struct OptionInfo
    {
        string name;
        string shortName;
        string description;
        string defaultValue_string; // for printing
        string current_value_string;
        vector<string> enum_values; // for enumerated values // TODO: make this into a more general constraint mechanism (?)

        vector<function<bool(string)>> set_value_callbacks; // callback returns "true" if value was successfully set, otherwise "false"
        vector<function<bool(string)>> did_set_value_callbacks; // callback returns "true" if value was successfully set, otherwise "false"

        // TODO: Add "policy" member (required, set_once, last_wins, etc.)

        OptionInfo()
        {}

        /// TODO: document
        ///
        OptionInfo(string name, string shortName, string description, string defaultValue, function<bool(string)> set_value_callback);

        // TODO: either have get_help_string() or print_help() methods for help and current value
    };

    // format of argv: Main.exe [options]
    // where options are of the form "-<string> <option>" where the <option> part is mandatory (defaulting to 'true')
    // options have two names, the short name is used with a single hyphen, and the long name with two
    // e.g., "-s true" and "--serial_mode true" can mean the same thing
    // options are queried by the long name
    // short name is optional
    // args are just strings at the end
    // ex of valid commandlines:
    // myexe.exe foo.tsv
    // myexe.exe foo.tsv bar.tsv
    // myexe.exe -t 8 -x blah foo.tsv bar.tsv
    class CommandLineParser
    {
    public:
    
        /// Constructor, takes arg list
        ///
        CommandLineParser(int argc, char**argv);
        
        /// AddOption adds a new option to the commandline parser
        ///
        template <typename T, typename U>
        void AddOption(T& option, string name, string shortName, string description, const U& defaultValue);

        /// Adds a string that gets printed out when pring_usage() is called
        ///
        virtual void AddDocumentationString(string str);

        /// Parses the commandline. Call this after setting up the options with AddOption
        ///
        void ParseArgs();

        /// TODO: document
        ///
        virtual void PrintUsage(ostream& out);

        /// TODO: document
        ///
        virtual void PrintCurrentValues(ostream& out);

        /// TODO: document
        ///
        bool HasOption(string option);

    protected:

        /// TODO: document
        ///
        CommandLineParser(const CommandLineParser&) = delete;

        /// TODO: document
        ///
        void SetArgs(int argc, char** argv);

        /// TODO: document
        ///
        template <typename T>
        static bool ParseVal(string str, T& result);

        /// TODO: document
        ///
        template <typename T>
        static bool ParseVal(string str, vector<pair<string, T>> val_names, T& result, string& result_string);

        /// TODO: document
        ///
        template <typename T>
        static string ToString(const T& val);

        struct DocumentationEntry    
        {
            enum type { option, str };
            type EntryType;
            string EntryString; // option name for option, docstring for string

            DocumentationEntry(type t, string str) : EntryType(t), EntryString(str) {}
        };

        vector<string> _originalArgs;
        string _exeName;
        vector<string> _args;
        map<string, string> _shortToLongNameMap;
        map<string, OptionInfo> _options;
        vector<DocumentationEntry> _docEntries;

        void AddOption(const OptionInfo& info);
        virtual bool SetOption(string option_name, string option_val); // returns true if we need to reparse
        bool SetDefaultArgs(const set<string>& unset_args); // returns true if we need to reparse
    };
}

#include "../tcc/CommandLineParser.tcc"