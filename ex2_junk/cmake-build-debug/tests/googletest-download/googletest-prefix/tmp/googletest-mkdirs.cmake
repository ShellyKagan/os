# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/T9137957/CLionProjects/os/ex2_junk/cmake-build-debug/tests/googletest-src"
  "C:/Users/T9137957/CLionProjects/os/ex2_junk/cmake-build-debug/tests/googletest-build"
  "C:/Users/T9137957/CLionProjects/os/ex2_junk/cmake-build-debug/tests/googletest-download/googletest-prefix"
  "C:/Users/T9137957/CLionProjects/os/ex2_junk/cmake-build-debug/tests/googletest-download/googletest-prefix/tmp"
  "C:/Users/T9137957/CLionProjects/os/ex2_junk/cmake-build-debug/tests/googletest-download/googletest-prefix/src/googletest-stamp"
  "C:/Users/T9137957/CLionProjects/os/ex2_junk/cmake-build-debug/tests/googletest-download/googletest-prefix/src"
  "C:/Users/T9137957/CLionProjects/os/ex2_junk/cmake-build-debug/tests/googletest-download/googletest-prefix/src/googletest-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/T9137957/CLionProjects/os/ex2_junk/cmake-build-debug/tests/googletest-download/googletest-prefix/src/googletest-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/T9137957/CLionProjects/os/ex2_junk/cmake-build-debug/tests/googletest-download/googletest-prefix/src/googletest-stamp${cfgdir}") # cfgdir has leading slash
endif()
