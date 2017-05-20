@page plugins Customizing Mast using plugins

[TOC]
[Acronyms]:   Acronyms.html

[API]:        Acronyms.html "Application Programming Interface"
[DLL]:        Acronyms.html "Dynamically Linked Library"
[JTAG]:       Acronyms.html "Joint Test Action Group"
[SUT]:        Acronyms.html "System Under Test"

[SIT]:        SIT_Files.html

@section intro Introduction

Mast behavior can be adapted to user needs using plugin extension points.
The customization is defined using object instance factories that are used while [SUT] description file - usually a [SIT] file - is parsed

It can be developed without linking to with MAST but it must comply to some factory classes and share the
same compiler ABI and libraries.

Plugins can be used to customize the following aspects have to use associated object factory:

| Aspects                   | Factory Classes                      | Descriptions |
| --------------------------|--------------------------------------|--------------|
| Configuration             | mast::ConfigureAlgorithmFactory      | Defines how to arbitrate requests to acces [SUT] i.e. select a linker path |
| Access interface protocol | mast::AccessInterfaceProtocolFactory | Defines how to interface with a local [SUT] hardware |
| Remote prototol           | mast::RemoteProtocolFactory          | Defines how to interface with a remote [SUT] hardware |
| Path Selector             | mast::PathSelectorFactory            | Provides a Linker selector (how to drive a mux) |


The simplest way, when using MAST provided applicaton, to use custom aspect(s) is to request to load a plugin [DLL] by giving the path to the plugin [DLL](s) in the configuration file.
The initialization part of the [DLL] - global static object(s) - must register plugin functions to associated factories.

@note A single [DLL] can provide several customization aspects

For cases when the application is build by the user, it can manages itself loading of plugin [DLL](s).
To do that, it simply include include Plugins.hpp header, use its trivial API and link to the plugins [DLL] helper,
That way it can easily browse for plugin [DLL]s, load a single plugin or all plugins found in a single directory.

To register plugin function(s) to a factory, just get the single instance of the factory and call its RegisterCreator method
with a creator identifier (std::string) and a function that creates an instance of the custom aspect.
Usually the creator function will use optional string representing its parameters (refer to the documentation of the factory for the exact prototypes of the function).

Mast have already pre-registered creational methods for "standard" needs. Those default aspects can be overriden
(by registering another method with the same identifier) or completly removed (using corresponding factory Clear method).


@section Default Default aspects

Configuration
-------------

| Identifier           | Description |
| ----------           | ----------- |
| LastOrDefault        | Selects pending path with highest identifier path that is not already selected and select path 0 when no path is selected. This is appropriate when registers are not modified between data cycles|
| LastOrDefault_Greedy | Same as LastOrDefault except it selected path 0 as soon as it is possible - which may select no path at all depending on the selector type -. This is appropriate for not frequent access |
| Last_Lazy            | Selects pending path with highest identifier that is not already selected - It never tries to select path 0 |


Access interface protocol
-------------------------

| Identifier                      | Description |
| ----------                      | ----------- |
| LoopbackAccessInterfaceProtocol | For tests purpose, acts as if [SUT] return exactly same bitstream it receives |
| SVF_SimulationProtocol          | Use SVF format to communicate to HDL simulator and receive back bitstream from the simulator |
| SVF_EmulationProtocol           | For tests purpose, this is a loopback with logging of SVF commands that would normally be issued with a real SVF interface|
| STIL_Emulation                  | For tests purpose, this is a loopback with logging of STIL commands that would normally be issued with a real STIL interface|
| I2C_EmulationProtocol           | For tests purpose, this is a loopback with logging of pseudo commands that would normally be issued with a real I2C interface|
| OfflineProtocol                 | Issue SVF commands when sending data [SUT], and read some binary file as data from [SUT] |
| RemoteProxy                     | Sends data to and receives from a remove interface |

Remote prototol
---------------

Currently there is no default remote protocol.

Path Selector
-------------

| Identifier     | Description                                                                    |
| ----------     | -----------                                                                    |
| Binary         | Binary encoded selector that support selecting no derivation at all            |
| Binary_noidle  | Binary encoded selector - always select a single derivation -                  |
| One_Hot        | One hot encoded selector that support selecting no derivation at all           |
| One_Hot_noidle | One hot encoded selector - always select a single derivation -                 |
| N_Hot          | N encoded selector that support selecting no derivation at all                 |
| N_Hot_noidle   | N hot encoded selector - always select at least one derivation -               |
| Table_Based    | Special encoding that use custom tables to define selection/deselection values |
