<p align="center">
  <img alt="pacemaker" src="./media/readme/pacemaker-logo-thumbnail.png" width="80%">
</p>

<div align="center">

[![C++](https://img.shields.io/badge/C++-%2300599C.svg?logo=c%2B%2B&logoColor=white)](#)
[![Windows](https://custom-icon-badges.demolab.com/badge/Windows-0078D6?logo=windows11&logoColor=white)](#)
[![License](https://img.shields.io/badge/license-MIT-blue)](LICENSE)
[![Contributor Covenant](https://img.shields.io/badge/Contributor%20Covenant-v2.1-ff69b4.svg)](CODE_OF_CONDUCT.md)

**COM automation for people with deadlines and a hatred of GUIs**

</div>

---

`pacemaker` is a lightweight C++ wrapper around the ETAS INCA COM API. It lets you programmatically register the calibration parameters, set their values in real time, and control measurement recordings, without touching INCA's GUI.

# Getting started

To start, clone the repository and move into the root directory
```console
git clone https://github.com/marcope-98/pacemaker.git
cd pacemaker
```

## Building Source Code

> [!NOTE]
> An MSVC compiler must be avaible on your system. For a portable alternative, see [PortableBuildTools by Data-Oriented-House](https://github.com/Data-Oriented-House/PortableBuildTools).

```console
cmake -B build -S . ^
      -DETAS_INCACOM_TLB="<path/to/incacom.tlb>" ^
      -DCMAKE_BUILD_TYPE="<Release|Debug|RelWithDebInfo|MinSizeRel>"

cmake --build build --target pacemaker
```

You can supply a `incacom.tlb` file via `ETAS_INCACOM_TLB` CMake variable, by specifying its absolute path. Otherwise a find module will take care of finding its location in regedit.

Due to backward compatibility issues, only ETAS INCA 7.4 and later installations are supported.

## Building and Running Unit Tests

Build the `tests` target and then run the test suite via CTest:
```console 
cmake -B build -S . ^
      -DPACEMAKER_BUILD_TESTS=ON

cmake --build build --target tests
ctest --test-dir build --output-on-failure
```

The CMake option `PACEMAKER_BUILD_TESTS` is `ON` by default in case the project is the top level project. 

To run a specific test by name or pattern, use the `-R` flag
```console
ctest --test-dir build --output-on-failure -R <test-name>
```

> [!TIP]
> Add `-j <N>` to run up to `N` tests in parallel, or `--rerun-failed` to only re-execute tests that failed in the previous run

## Generating Requirements Documents
The Software Requirements Specification (SRS) and Test Specification (TS) documents are written in LaTeX and built using `latexmk` perl script via MiKTeX.

**Dependencies**:
 - [MiKTeX](https://miktex.org/download): LaTeX distribution (install `latexmk` via its package manager)
 - [Strawberry perl](https://strawberryperl.com/): Perl runtime for Windows

```console
cmake -B build -S . ^
      -DMIKTEX_BINARY_PATH="<path/tp/miktex/bin>" ^
      -DPACEMAKER_BUILD_REQUIREMENTS=ON

cmake --build build --target requirements
```

`MIKTEX_BINARY_PATH` is only required for portable MiKTeX installations.

The CMake option `PACEMAKER_BUILD_REQUIREMENTS` is `ON` by default in case the project is the top level project. 

## Generating Source Documentation
API documentation is generated with [Doxygen](https://www.doxygen.nl/) and [Graphviz](https://graphviz.org/).

```console
cmake -B build -S . ^
      -DDOXYGEN_EXECUTABLE="<path/to/doxygen>" ^
      -DDOXYGEN_DOT_EXECUTABLE="<path/to/dot>" ^
      -DPACEMAKER_BUILD_DOCS=ON

cmake --build build --target docs
```

> [!NOTE]
> `DOXYGEN_EXECUTABLE` and `DOXYGEN_DOT_EXECUTABLE` are only required for portable installations

The CMake option `PACEMAKER_BUILD_REQUIREMENTS` is `ON` by default in case the project is the top level project. 

## Building and Running Examples
A Command Line Interface example is provided that uses all the implemented components of the `pacemaker` library.

```console
cmake -B build -S . ^
      -DPACEMAKER_BUILD_EXAMPLES=ON

cmake --build build --target examples

.\build\examples\pacemaker-cli <period> <csv_file> [<output file or folder>]
```

The CMake option `PACEMAKER_BUILD_EXAMPLES` is `ON` by default in case the project is the top level project. 

The CLI reads the csv file that has the following form:
- Header: comma separated names of the INCA parameters to change (quoted or unquoted)
- Values: comma separated double values to set the INCA parameters to

Each row of the Values refers to a different sample. The period of execution of each row is dictated by the first CLI argument that has to be the period in milliseconds.

Putting all things together, suppose I would like to execute every row of a csv file called `thefile.csv` at 10ms intervals, I would call the CLI with the following syntax:
```console
.\build\examples\pacemaker-cli 10ms .\thefile.csv
```

Optionally, once can specify the output file or folder for the recording:
- If one omit the argument, the recording will be saved in the default location with the default name
- If one provides a folder location, the recording will be saved at the specified location with the default name
- If one provides the full path and filename, the recording will be saved with the provided name and location.

# Contributing
We welcome contributions from everyone in the community! To get started, please read our [CONTRIBUTING.md](CONTRIBUTING.md) guide. Whether you're adding a new feature, improving documentation, or fixing a bug, your help and feedback are invaluable. 

# Authors
- Marco Peressutti - [@marcope-98](https://github.com/marcope-98) 
