# pacemaker Documentation
`pacemaker` is a lightweight C++ wrapper around the ETAS INCA COM API. It lets you programmatically register the calibration parameters, set their values in real time, and control measurement recordings, without touching INCA's GUI.

## pacemaker Directory Structure
```console
pacemaker
├───.github
│   └───ISSUE_TEMPLATE
├───cmake
├───docs
├───examples
├───media
│   └───readme
├───requirements
│   ├───chapters
│   │   ├───REQ
│   │   └───TC
│   └───lib
├───src
│   ├───include
│   │   └───pacemaker
│   │       ├───inca
│   │       └───timer
│   └───src
│       └───pacemaker
│           ├───inca
│           │   ├───com
│           │   └───detail
│           └───timer
└───tests
    └───pacemaker
        ├───fixture
        ├───inca
        └───timer
```
## Getting started

To start, clone the repository and move into the root directory
```console
git clone https://github.com/marcope-98/pacemaker.git
cd pacemaker
```

### Building Source Code

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


### Building and Running Unit Tests

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

### Generating Requirements Documents
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

### Generating Source Documentation
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

## AI Policy

The pacemaker project welcomes contributions from everyone, and we have a few guidelines regarding AI usage to ensure high code quality, clear communication, and a healthy open-source ecosystem:

- **Please disclose significant AI assistance.** If you used AI tools (e.g., Copilot, Claude, Cursor, ChatGPT) to generate a substantial portion of your code or text, let us know in your PR description. Transparency helps us review your changes more effectively.
- **Commits and PRs content**. Commits that include AI generated content **MUST** explicitly refer to the provider, the model name and version used via the `Co-authored-by: ` convention in the commit body. (e.g. `Co-authored-by: anthropic/claude-sonnet-4.6 <noreply@anthropic.com>`)  
- **Own your code (The Human-in-the-Loop).** You must fully understand all the changes you are proposing. If you cannot explain what your AI-assisted code does or how it interacts with pacemaker's broader architecture, please take the time to learn and test it before submitting.
- **Keep issues and discussions focused.** You are welcome to use AI to help draft issues or PR descriptions, but please review and edit them carefully before posting. AI can often be overly verbose; trimming the noise and getting straight to the point helps our maintainers address your needs faster.

Our core maintainers also use AI tools to aid their workflows, but they do so while bringing deep contextual knowledge of the pacemaker codebase to validate the output. We ask all contributors to apply that same level of rigor.

### Remember the Human Maintainers

Please remember that pacemaker is maintained by a dedicated team of humans.

Every discussion, issue, and pull request is read and reviewed by real people. While AI tools can generate thousands of lines of code in seconds, reviewing that code still takes human time and energy. Submitting unverified or low-effort AI output puts an unfair burden on our maintainers.

Today, the quality of the AI output still heavily depends on the developer driving the tool. We ask that you respect our maintainers' time by thoroughly vetting, testing, and refining your submissions.

### AI is Welcome Here

pacemaker operates at the cutting edge of AI and robotics, and many of our maintainers actively embrace AI coding assistants as valuable productivity tools. We are a pro-AI project!

Our reason for having an AI policy is not an anti-AI stance. Rather, it exists to ensure that AI is used to enhance human contributions, not replace them with unverified noise. It's about how the tools are used, not the tools themselves.

We value the unique human insight you bring to the pacemaker community. Let AI empower your workflow, but always let your own judgment take the wheel.

## License

MIT License

Copyright (c) 2026 Marco Peressutti

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
