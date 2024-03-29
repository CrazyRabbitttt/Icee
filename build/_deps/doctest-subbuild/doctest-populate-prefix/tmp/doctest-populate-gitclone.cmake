
if(NOT "/Users/shaoguixin/Icee/build/_deps/doctest-subbuild/doctest-populate-prefix/src/doctest-populate-stamp/doctest-populate-gitinfo.txt" IS_NEWER_THAN "/Users/shaoguixin/Icee/build/_deps/doctest-subbuild/doctest-populate-prefix/src/doctest-populate-stamp/doctest-populate-gitclone-lastrun.txt")
  message(STATUS "Avoiding repeated git clone, stamp file is up to date: '/Users/shaoguixin/Icee/build/_deps/doctest-subbuild/doctest-populate-prefix/src/doctest-populate-stamp/doctest-populate-gitclone-lastrun.txt'")
  return()
endif()

execute_process(
  COMMAND ${CMAKE_COMMAND} -E rm -rf "/Users/shaoguixin/Icee/build/_deps/doctest-src"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to remove directory: '/Users/shaoguixin/Icee/build/_deps/doctest-src'")
endif()

# try the clone 3 times in case there is an odd git clone issue
set(error_code 1)
set(number_of_tries 0)
while(error_code AND number_of_tries LESS 3)
  execute_process(
    COMMAND "/opt/homebrew/bin/git"  clone --no-checkout --depth 1 --no-single-branch --config "advice.detachedHead=false" "https://gitee.com/acking-you/doctest.git" "doctest-src"
    WORKING_DIRECTORY "/Users/shaoguixin/Icee/build/_deps"
    RESULT_VARIABLE error_code
    )
  math(EXPR number_of_tries "${number_of_tries} + 1")
endwhile()
if(number_of_tries GREATER 1)
  message(STATUS "Had to git clone more than once:
          ${number_of_tries} times.")
endif()
if(error_code)
  message(FATAL_ERROR "Failed to clone repository: 'https://gitee.com/acking-you/doctest.git'")
endif()

execute_process(
  COMMAND "/opt/homebrew/bin/git"  checkout v2.4.9 --
  WORKING_DIRECTORY "/Users/shaoguixin/Icee/build/_deps/doctest-src"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to checkout tag: 'v2.4.9'")
endif()

set(init_submodules TRUE)
if(init_submodules)
  execute_process(
    COMMAND "/opt/homebrew/bin/git"  submodule update --recursive --init 
    WORKING_DIRECTORY "/Users/shaoguixin/Icee/build/_deps/doctest-src"
    RESULT_VARIABLE error_code
    )
endif()
if(error_code)
  message(FATAL_ERROR "Failed to update submodules in: '/Users/shaoguixin/Icee/build/_deps/doctest-src'")
endif()

# Complete success, update the script-last-run stamp file:
#
execute_process(
  COMMAND ${CMAKE_COMMAND} -E copy
    "/Users/shaoguixin/Icee/build/_deps/doctest-subbuild/doctest-populate-prefix/src/doctest-populate-stamp/doctest-populate-gitinfo.txt"
    "/Users/shaoguixin/Icee/build/_deps/doctest-subbuild/doctest-populate-prefix/src/doctest-populate-stamp/doctest-populate-gitclone-lastrun.txt"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to copy script-last-run stamp file: '/Users/shaoguixin/Icee/build/_deps/doctest-subbuild/doctest-populate-prefix/src/doctest-populate-stamp/doctest-populate-gitclone-lastrun.txt'")
endif()

