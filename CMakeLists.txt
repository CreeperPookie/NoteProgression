cmake_minimum_required(VERSION 3.30)
project(NoteProgression)

include(CTest)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "-fno-builtin-round -O0 ${CMAKE_CXX_FLAGS} -std=c++${CMAKE_CXX_STANDARD}")
set(CMAKE_CXX_STANDARD_LIBRARIES, "-lSDL2")
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")

include_directories(.)
include_directories(note)
include_directories(util)

add_executable(NoteProgression
        note/GenerationMode.cpp
        note/GenerationMode.h
    note/Instrument.cpp
    note/Instrument.h
    note/Note.cpp
    note/Note.h
    note/NoteType.cpp
    note/NoteType.h
        note/Panning.cpp
    note/Panning.h
    util/BinaryFile.cpp
    util/BinaryFile.h
    NoteProgression.cpp
    NoteProgression.h
        util/Utility.cpp
        util/Utility.h
        Error.h
        Error.cpp
        util/Random.cpp
        util/Random.h
        note/random/RandomPattern.cpp
        note/random/RandomPattern.h
        note/random/RandomPatternSegment.cpp
        note/random/RandomPatternSegment.h
        note/random/RandomData.h
        util/OutputData.h
)
add_test(NAME NoteProgressionTester
        COMMAND NoteProgressionTester)
set(passRegex "^Success" "^All tests passed")
set(failRegex "^Error" "^Unable parse all note tests!")