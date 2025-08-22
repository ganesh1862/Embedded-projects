# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Espressif/frameworks/esp-idf-v5.3.1/components/bootloader/subproject"
  "F:/Learn/weather_report/build/bootloader"
  "F:/Learn/weather_report/build/bootloader-prefix"
  "F:/Learn/weather_report/build/bootloader-prefix/tmp"
  "F:/Learn/weather_report/build/bootloader-prefix/src/bootloader-stamp"
  "F:/Learn/weather_report/build/bootloader-prefix/src"
  "F:/Learn/weather_report/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "F:/Learn/weather_report/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "F:/Learn/weather_report/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
