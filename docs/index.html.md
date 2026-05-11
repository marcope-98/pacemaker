# pacemaker Documentation
## About pacemaker
## pacemaker Directory Structure

## Building Instructions
### Building Source Code
The project is based on CMake. 

To start, clone the repository
```console
git clone https://github.com/marcope-98/pacemaker.git
cd pacemaker
```
> [!NOTE]
> Make sure an MSVC compiler is available on your system. For a portable alternative consider [PortableBuildTools by Data-Oriented-House](https://github.com/Data-Oriented-House/PortableBuildTools).

Create the build directory and run cmake by specifying the location of the `incacom.tlb` file available in your ETAS INCA installation folder.
```console
cmake -B build ^
      -S . ^
      -DINCACOM_TLB="..."
```
Finally compile the code
```console
cmake --build build --target pacemaker
```
### Building and Running tests
### Building and Running examples
### Generating Requirements Documents
The Software Requirements Specification (SRS) and Test Specification (TS) documents are written using LaTeX and are built using latexmk perl script from the MiKTeX Tex distribution.

> [!NOTE]
> For the MiKTeX installation instructions please visit the link [MiKTeX Download](https://miktex.org/download).
>
> For an easy to install perl environment for MS Windows please visit the link [Strawberry perl](https://strawberryperl.com/)

Once the dependencies are in place, and latexmk is installed via the MiKTeX package manager the SRS and TS documents can be generated via CMake.

In case of a portable version of MiKTeX you can supply the path via the `MIKTEX_BINARY_PATH` CMake variable.

Finally, enable building the documents by setting the variable `PACEMAKER_BUILD_REQUIREMENTS` to `ON`.

```console
cmake -B build ^
      -S . ^
      -DINCACOM_TLB="..." ^
      -DMIKTEX_BINARY_PATH="..." ^
      -DPACEMAKER_BUILD_REQUIREMENTS=ON
``` 

Then build the `requirements` target
```console
cmake --build build --target requirements
```

### Generating Source Documentation
The source documentation is generated automatically with Doxygen and Graphviz. Therefore a valid installation of both tool must be available.

If you have a portable version of these tools consider using the flags `DOXYGEN_EXECUTABLE` and/or `DOXYGEN_DOT_EXECUTABLE`.

Finally, enable building source code documentation by setting the variable `PACEMAKER_BUILD_DOCS` to `ON`
```console
cmake -B build ^
      -S . ^
      -DINCACOM_TLB="..." ^
      -DDOXYGEN_EXECUTABLE="..." ^
      -DDOXYGEN_DOT_EXECUTABLE="..." ^
      -DPACEMAKER_BUILD_DOCS=ON
```

And finally build the `docs` target
```console
cmake --build build --target docs
```
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
