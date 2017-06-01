Mast Options  {#MastOptions}
============

[TOC]

[GML]:   Acronyms.html "Graph Modelling Language"
[MAST]:  Acronyms.html "MAnager for SoC Test"
[PDL]:   Acronyms.html "Procedure Description Language"
[SUT]:   Acronyms.html "System Under Test"

[SIT]:    SIT_Files.html
[plugin]: Plugins.html

# Configuration File # {#Config_File}

Configuration file follows [YAML](https://fr.wikipedia.org/wiki/YAML) syntax.

Default configuration file looks like this:

     Mast_Options:
       SIT_file_path: project.sit               # [SIT] file used by the project
       Configuration_algorithm: last_or_default # One of [last_lazy, last_or_default, last_or_default_greedy] or one defined by a [plugin])
       Access_interface_protocol: SIT           # When different to "SIT", it override the one define in [SIT] file
       Plugin_DLLs:  Plugins                    # Root path and/or list of file(s) path
       Model_checking: true                     # Enable/Disable model checking (provided it has been parsed successfully)
         File_path: ""                          # Optional file path (logged when no path when logged is enabled)
       Debug:
         Logging:
           Enable: true                         # Enable/Disable general logging
           Logger_Kind: std                     # One of [std, copy_all_on_cout, copy_errors_on_cerr]
           File_path:  ""                       # File path for logging (there is always a file logging when enabled)
           Shown_items: std                     # Any of [date, time, microseconds, level, thread_id, file_name, line_number, function_name, std, all]
           Level: info                          # One of [debug, info, warning, error]
         Model_GML_printing: false              # Enable/Disable printing of GML representation of the model state
           Moments:                             # Any of [after_model_parsing, before_configuration, after_configuration] (see bellow)
             - After_Parsing
           File_path: ""                        # Optional file path for GML graph printing
           Graph_name:      ""                  # Optional graph name
           Options: std                         # Any of [default, identifiers, register_value, auto_value, protocol, selector_with_edge, selector_properties, selector_tables, selection_values, std, all]
         Model_textual_print: false             # Enable/Disable printing of textual representation of the model state
           Moments:                             # Any of [after_model_parsing, before_configuration, after_configuration] (see bellow)
             - After_Parsing
           File_path: ""                        # Optional file path
           Options:  default                    # Any of [default, verbose, auto_value, protocol_name, selection_state, selection_value, selector_properties, ignored_nodes, std, all]
         Manager_activity:  false               # Enable/Disable logging and optionally reporting model state at specific point of manager activity
           File_base_name: ""                   # Path where pretty print and/or GML files are saved
           Options: PDL_commands                # Any of [verbose, app_thread_creation, PDL_commands, data_cycles, std, all]
     Plugins_Options: ""                        # Defines plugins specific options (ignored by Mast)

Same default values are used when no configuration file is provided nor found in current directory.


# Command Line # {#Cmd_Line}

Command line follows the Linux [Program Argument Syntax Conventions](https://www.gnu.org/software/libc/manual/html_node/Argument-Syntax.html).

Command line switches override same value in configuration file.

Usage is:

    Mast [--conf= _path_to_configuration_file_] [other_options]

| Short | Long | Default value | Description |
| ----- | ---- | ------------- | ----------- |
| -h  | --help        | n/a             | Display command line help and exit
| -v  | --version     | n/a             | Display Mast version and exit
| -c  | --conf        | mast.cfg        | Define configuration file
| -s  | --sit         | project.sit     | Define [SIT] that specified [SUT] model
| -a  | --config_algo | last_or_default | Name of configuration algorithm used to select linker (mux) path
| n/a | --protocol    | [SIT]           | Override access interface protocol defined in [SIT] file
| n/a | --plugin      | plugins         | Define [plugin]s to load (may be directory of file path) - Can be repeated as much as necessary
| n/a | --check       | n/a             | Enable model checking (resulting from parsing [SIT] file)
| n/a | --check_file  | mast_check.txt  | Defines result of model checking (it is always logged when logger is enabled)
| -l  | --log         | n/a             | Enable logger
| n/a | --log_file    | mast.log        | Define logger file path
| n/a | --log_level   | info            | Define log level: one of [debug, info, warning, error]
| n/a | --log_kind    | std             | Define logger kind: one of [std, copy_all_on_cout, copy_errors_on_cerr]

If there is a `mast.cfg` file in current working directory, it will be used as the configuration file.

----

# Options details # {#Option_Details}

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

| | |
|-|-|
| after_model_parsing  | Just after [SIT] file has been parsed
| before_configuration | Monitor System_model state before each configuration  (when manager activity reporting is enabled)
| after_configuration  | Monitor System_model state after each configuration   (when manager activity reporting is enabled)

### GML options  {#GML_Options}

| | |
|-|-|
| default             | Minimal graph is produced
| identifiers         | To show node identifier
| register_value      | To show Register values
| auto_value          | To show Register values as binary when small, hexa when large and end of large string as binary when cannot form a plain nibble
| protocol            | To show Protocol name of an access interface
| selector_with_edge  | To show Linker selector associated register with an edge between the Linker and the Register (this can cause out of order display)
| selector_properties | To show selector properties
| selector_tables     | To show selector tables for selection/deselection
| selection_values    | To show Selector Register value along edge between linker and derivations
| std                 | Equivalent to [identifiers, register_value, auto_value, selection_values]
| all                 | Equivalent to all options (except for default)

### Textual options {#Textual_Options}

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
| | |
|-|-|
| std                 | Messages are logged only to log file
| copy_all_on_cout    | Messages are logged to log file AND to the console
| copy_errors_on_cerr | Messages are logged to log file AND error messages also to the console

### Shown items {#Logger_Shown_Items}
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
| std           | Equivalent to [time, microseconds, level, thread_id]
| all           | Equivalent to all options

### Level {#Logger_Level}
| | |
|-|-|
| debug   | All messages are logged (including debug ones)
| info    | Informational, warning and error messages are logged
| warning | Warning and error messages are logged
| error   | Only error messages are logged

