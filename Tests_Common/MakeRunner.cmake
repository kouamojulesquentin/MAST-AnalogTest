# MakeRunner
#

if (WIN32)
 set(python $ENV{PYTHON3})
 if (NOT python)
  message(STATUS "MakeRunner: PYTHON3 environment variable is not defined ==> Will try with Python3")
  set(python $ENV{Python3})
  if (NOT python)
    message(WARNING "Python is not defined")
  endif()
 endif()
else()  # ==> Linux
  set(python python3)
endif()


function(MakeRunner cxxtestPath runnerPath extraDepends headers)

  set(generator       ${cxxtestPath}/bin/cxxtestgen.py)
  set(templateFile    ${CMAKE_CURRENT_SOURCE_DIR}/Runner.tpl)
  set(generatorParams --error-printer --have-eh --have-std --fog-parse --root -o ${runnerPath} --template ${templateFile})

  message(STATUS "MakeRunner: python:                 ${python}")
  message(STATUS "MakeRunner: CMAKE_CURRENT_LIST_DIR: ${CMAKE_CURRENT_LIST_DIR}")
  message(STATUS "MakeRunner: generator:              ${generator}")
  message(STATUS "MakeRunner: extraDepends:           ${extraDepends}")
  message(STATUS "MakeRunner: templateFile:           ${templateFile}")
  message(STATUS "MakeRunner: generatorParams:        ${generatorParams}")
  message(STATUS "MakeRunner: headers:")

  foreach(item ${headers})
    message(STATUS "MakeRunner:                         ${item}")
  endforeach()

  add_custom_command (OUTPUT    ${runnerPath}
                      DEPENDS   ${headers} ${extraDepends} ${templateFile}
                      COMMAND   ${python}  ${generator} ${generatorParams} ${headers}
                     )

  set_source_files_properties(${runnerPath} GENERATED)

endfunction(MakeRunner)
