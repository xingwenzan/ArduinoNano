# !!! WARNING !!! AUTO-GENERATED FILE, PLEASE DO NOT MODIFY IT AND USE
# https://docs.platformio.org/page/projectconf/section_env_build.html#build-flags
#
# If you need to override existing CMake configuration or add extra,
# please create `CMakeListsUser.txt` in the root of project.
# The `CMakeListsUser.txt` will not be overwritten by PlatformIO.

cmake_minimum_required(VERSION 3.13)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

project("ArduinoNano" C CXX)

include(CMakeListsPrivate.txt)

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/CMakeListsUser.txt)
    include(CMakeListsUser.txt)
endif()

add_custom_target(
        Production ALL
        COMMAND platformio -c clion run "$<$<NOT:$<CONFIG:All>>:-e${CMAKE_BUILD_TYPE}>"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(
        Debug ALL
        COMMAND platformio -c clion debug "$<$<NOT:$<CONFIG:All>>:-e${CMAKE_BUILD_TYPE}>"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(Z_DUMMY_TARGET ${SRC_LIST}
        include/Init/LedBlink.cpp
        include/Init/LedBlink.cpp
        include/Encapsulation/SawtoothWave.cpp
        include/Init/OpenAndCloseLED.cpp
        include/Encapsulation/WaveOutput.cpp
        include/Encapsulation/CustomWaveInitial.cpp
        include/Encapsulation/NewCustom.cpp
        include/Encapsulation/WaveformAndSpectrum.cpp
        include/Init/InitLcdAndKeypad.cpp
        include/Encapsulation/NewCtrlCustom.cpp
        include/Init/ClockDisplay.cpp
        include/Integrations/ClockWaveAndScale.cpp
        include/Init/U8g2Helloworld.cpp
        lib/TM1638/src/TM1638.cpp
        include/Init/TM1638Hellowirld.cpp
        include/Init/DebugFunction.cpp
        include/Integrations/DigitalDisplay.cpp
        lib/Ucglib/src/Ucglib.h
        lib/TimerOne/TimerOne.cpp
        include/Init/TimerInterruptLib.cpp
        include/Tmp/WIFIHelloworldTmp.cpp
        include/Tmp/TFTLCDTestTmp.cpp
        include/Init/TimerInterruptOriginal.cpp
        include/Init/UltrasonicRanging.cpp
        include/Init/StepperInit.cpp
        include/Communication/LedBlinkAndSerial.cpp
        include/Communication/SquareWaveFrequencyMeasurement.cpp)
