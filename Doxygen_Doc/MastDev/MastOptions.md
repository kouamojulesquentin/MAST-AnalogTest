Mast Options  {#MastOptions}
============

[TOC]

[GML]:   Acronyms.html "Graph Modelling Language"
[MAST]:  Acronyms.html "MAnager for SoC Test"
[PDL]:   Acronyms.html "Procedure Description Language"
[SUT]:   Acronyms.html "System Under Test"

[SIT]:     SIT_Files.html
[plugin]:  Plugins.html
[plugins]: Plugins.html

# Introduction # {#Intro}

[MAST] can be configured in using command line arguments, configuration files and [plugins]. <br>
Indication of plugin(s) can be done using configuration file or command line arguments.
Configuration file options override built-in default value and command line argument override configuration file options.
So option can be freely defined through a mix of configuration file options and command line arguments.

# Configuration File # {#Config_File}

Configuration file follows [YAML](https://fr.wikipedia.org/wiki/YAML) syntax.

Default configuration file looks like this:

    Mast_Options:
      SIT_file_path:                                          # [SIT] file used by the project
      Access_interface_protocol:
        Name:                                                 # When defined, it overrides the one defined in [SIT] file
        Parameters:                                           # Optional access interface protocol parameters (in case the one in [SIT] file is overriden)
      Configuration_algorithm: last_or_default                # One of [last_lazy, last_or_default, last_or_default_greedy] or one defined by a [plugin])
      Plugins:
        Files: []                                             # List of plugin file(s) path (absolute or relative path)
        Directories: [Plugins]                                # Directory from where to load plugin files (absolute or relative path)
      Model_checking:
        Enable:    false                                      # Enable/Disable model checking (provided it has been parsed successfully)
        File_path: ""                                         # Optional file path (logged when no path when logged is enabled)
      Manager:
        Min_time_between_cycles: 10ms                         # Minimal time between two I/O cycles
        Max_time_between_cycles: 1s                           # Maximal time between I/O cycles. It corresponds to the maximal delay between an iApply assertion and its execution
      Debug:
        Logging:
          Enable: false                                       # Enable/Disable general logging
          Kind:   [file]                                      # One of [file, cout, only_errors_on_cerr]
          File_path:  "Mast.log"                              # File path for logging (there is always a file logging when enabled)
          Shown_items: [std_less]                             # Any of [date, time, microseconds, level, thread_id, file_name, line_number, function_name, std_less, std, std_more,all]
          Level: info                                         # One of [debug, info, warning, error]
        Model_GML_printing:
          Enable:  false                                      # Enable/Disable printing of GML representation of the model state
          Moments:                                            # Any of [after_model_parsing, before_configuration, after_configuration] (see bellow)
            - after_model_parsing
          File_path:   "MastModel.gml"                        # Optional file path for GML graph printing
          Graph_name:  "DUT"                                  # Optional graph name
          Options: [std]                                      # Any of [default, identifiers, register_value, auto_value, protocol_name, selector_with_edge, selector_properties, selector_tables, selection_value, std, all]
        Model_textual_print:
          Enable:  false                                      # Enable/Disable printing of textual representation of the model state
          Moments:                                            # Any of [after_model_parsing, before_configuration, after_configuration] (see bellow)
            - after_model_parsing
          File_path: "MastModel.txt"                          # Optional file path
          Options:   [std]                                    # Any of [default, verbose, auto_value, protocol_name, selection_state, selection_value, selector_properties, ignored_nodes, std, all]
        Manager_activity:
          Enable:         false                               # Enable/Disable logging and optionally reporting model state at specific point of manager activity
          File_base_name: "DUT"                               # Path where pretty print and/or GML files are saved
          Options:        [app_thread_creation, PDL_commands] # Any of [verbose, app_thread_creation, PDL_commands, data_cycles, std, all]
    Plugins_Options: ""                                       # Defines plugins specific options (ignored by Mast)

Same default values are used when no configuration file is provided nor found in current directory.


# Command Line # {#Cmd_Line}

Command line follows the Linux [Program Argument Syntax Conventions](https://www.gnu.org/software/libc/manual/html_node/Argument-Syntax.html).

Command line switches override same value in configuration file.

Usage is:
      Mast.exe  [-c=<File path>]
                [-s=<File path>]
                [--plugin_dir=<Directory path>] ...
                [--plugin=<File path>] ...
                [-l]
                [--log_file=<File path>]
                [--log_show=<all|date|file_name|function_name|level|line_number|microseconds|std|std_less|std_more|thread_id|time>] ...
                [--log_level=<debug|error|info|warning>] ...
                [--log_kind=<cout|file|only_errors_on_cerr>] ...
                [--check]
                [--check_file=<File path>]
                [-a=<last_lazy|last_or_default|last_or_default_greedy| "name defined by a plugin">]
                [--max_cycle=<Positive integer with 'ms' or 's' suffix>]
                [--min_cycle=<Positive integer with 'ms' or 's' suffix>]
                [--protocol_parameters=<Protocol parameter(s)>]
                [--protocol_name=<Protocol name>]
                [--]


    Where:

      Mast: Manager for System On Chip Tests

      -c=<File path>,  --conf=<File path>
        Defines configuration file

      -s=<File path>,  --sit=<File path>
        Defines SIT that specified SUT model

      --plugin_dir=<Directory path>  (accepted multiple times)
        Defines a plugin directory (all plugins in it are loaded)

      --plugin=<File path>  (accepted multiple times)
        Defines a plugin file to load

      -l,  --log
        Enables logger

      --log_file=<File path>
        Defines logger file path

      --log_show=<all|date|file_name|function_name|level|line_number|microseconds|std|std_less|std_more|thread_id|time>  (accepted multiple times)
        Defines log shown items

      --log_level=<debug|error|info|warning>  (accepted multiple times)
        Defines log level

      --log_kind=<cout|file|only_errors_on_cerr>  (accepted multiple times)
        Defines logger kind

      --check
        Enables model checking (resulting from parsing SIT file)

      --check_file=<File path>
        Defines result of model checking (it is always logged when logger is enabled)

      -a=<last_lazy|last_or_default|last_or_default_greedy| "name defined by a plugin">,
      --config_algo=<last_lazy|last_or_default|last_or_default_greedy| "name defined by a plugin">
        Name of configuration algorithm used to select linker  (mux) path

      --max_cycle=<Positive integer with 'ms' or 's' suffix>
        Maximal time to take into account an iApplyMaximal time between I/O cycles.
        It corresponds to the maximal delay between an iApply assertion and its execution

      --min_cycle=<Positive integer with 'ms' or 's' suffix>
        Minimal time between two I/O cycles

      --protocol_parameters=<Protocol parameter(s)>
        Optional access interface protocol parameters

      --protocol_name=<Protocol name>
        Overrides access interface protocol defined in SIT file

      --,  --ignore_rest
        Ignores the rest of the labeled arguments following this flag.


If there is a `mast.cfg` file in current working directory, it will be used as the configuration file.

----

# Options details # {#Option_Details}

## Plugins ## {#plugins}

This option controls which plugins are loaded at startup. <br>
Loading is done following this order:
 - Plugins given at Command-line
 - Plugins contained in directories given at Command-line
 - Plugins given in Configuration file
 - Plugins contained in directory given in Configuration file
 - Plugins in current directory
 - Plugins in MAST root "Plugins" directory


## Configuration algorithm ## {#Config_Algo}

Name of configuration algorithm used to select linker (mux) path to arbritrate access from different test algorithms.
This can be one of Mast built-in algorithm (see bellow) or one defined by a [plugin]

| | |
|-|-|
| last_or_default        | Selects pending path with highest identifier path that is not already selected and select path 0 when no path is selected. This is appropriate when registers are not modified between data cycles
| last_or_default_greedy | Same as LastOrDefault except it selected path 0 as soon as it is possible - which may select no path at all depending on the selector type -. This is appropriate for not frequent access
| last_lazy              | Selects pending path with highest identifier that is not already selected - It never tries to select path 0

## State printing {#State_Printing}
### Moments       {#Moments}

Defines moment(s) in the vector generation for which model state is reported

| | |
|-|-|
| after_model_parsing  | Just after [SIT] file has been parsed
| before_configuration | Monitor System_model state before each configuration  (when manager activity reporting is enabled)
| after_configuration  | Monitor System_model state after each configuration   (when manager activity reporting is enabled)

### GML options  {#GML_Options}

Defines what kind of model information pieces are displayed in the [GML] model state report.

| | |
|-|-|
| default             | Minimal graph is produced
| identifiers         | To show node identifier
| register_value      | To show Register values
| auto_value          | To show Register values as binary when small, hexa when large and end of large string as binary when cannot form a plain nibble
| protocol_name       | To show Protocol name of an access interface
| selector_with_edge  | To show Linker selector associated register with an edge between the Linker and the Register (this can cause out of order display)
| selector_properties | To show selector properties
| selector_tables     | To show selector tables for selection/deselection
| selection_values    | To show Selector Register value along edge between linker and derivations
| std                 | Equivalent to [identifiers, register_value, auto_value, selection_values]
| all                 | Equivalent to all options (except for default)

### Textual options {#Textual_Options}

Defines what kind of model information pieces are reported in the textual model state report.

| | |
|-|-|
| default             | Minimal information is produced
| verbose             | To report whether a node is pending, has conditioner, is ignored for path specification, its priority, protocol name for access interface, all values for registers...
| auto_value          | To report Register values as binary when small, hexa when large and end of large string as binary when cannot form a plain nibble
| protocol_name       | To report Protocol name of an access interface
| selection_state     | To report whether a node is selected or active
| selection_value     | To report which selector value select the nodes
| selector_properties | To report selector properties (kind, can_select_none, inverted_bits, reverted_order...)
| ignored_nodes       | To report parent node that are ignored when defining a node path
| std                 | Equivalent to [verbose, auto_value]
| all                 | Equivalent to all options (except for default)

### Manager activity  {#Manager_Activity}

Defines what Mast manager main activities are reported in the log
| | |
|-|-|
| verbose              | Log more information
| app_thread_creation  | Log application thread creation
| [PDL]_commands       | Log [PDL] commands issued by applications
| data_cycles          | Log data cycle main events
| std                  | Equivalent to [app_thread_creation, [PDL]_commands]
| all                  | Equivalent to all options (except for default)


## Logger  {#logger}
### Kind   {#Logger_Kind}

Defines what kind of logger is used.

| | |
|-|-|
| std                 | Messages are logged only to log file
| cout                | Messages are logged only to cout
| copy_all_on_cout    | Messages are logged to log file AND to the console
| copy_errors_on_cerr | Messages are logged to log file AND error messages also to the console

### Shown items {#Logger_Shown_Items}

Defines what are the auxiliary elements reported in the log.

| | |
|-|-|
| date          | Show (calendar) date of message
| time          | Show (clock) time of message
| microseconds  | Show microsecond of message (from start of logger)
| level         | Show message level (debug, info, warning, error)
| thread_id     | Show thread identifier from which the message was logged
| file_name     | Show file name from which the message was logged (for Mast developer team)
| line_number   | Show source line number from which the message was logged (for Mast developer team)
| function_name | Show function name from which the message was logged (for Mast developer team)
| std_less      | Equivalent to [level, thread_id]
| std           | Equivalent to [microseconds, level, thread_id]
| std_more      | Equivalent to [function_name, microseconds, level, thread_id]
| all           | Equivalent to all options

### Level {#Logger_Level}

Defines level of information that is logged.

| | |
|-|-|
| debug   | All messages are logged (including debug ones)
| info    | Informational, warning and error messages are logged
| warning | Warning and error messages are logged
| error   | Only error messages are logged

