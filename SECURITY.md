# Security Policy

## Supported Versions

`pacemaker` follows semantic versioning. Security fixes are backported only to the latest major release line; everyone else should upgrade.

| Version | Supported          |
| ------- | ------------------ |
| 3.x     | :white_check_mark: |
| 2.x     | :x:                |
| < 2.0   | :x:                |

If you're building against an older tag, please update to the latest `3.x` release before reporting an issue. It may already be fixed.

## Reporting a Vulnerability

**Please do not open a public Github issue for security vulnerabilities.**

Instead, report it privately using [Github Security Advisories](https://github.com/marcope-98/pacemaker/security/advisories/new). Use the "Report a vulnerability" button under this repository's Security tab.

When reporting, please include as much of the following as you can:
- The affected version, tag or commit hash
- A description of the vulnerability and its potential impact
- Steps to reproduce or a minimal proof of concept
- Any suggested mitigation of fix, if you have one

### What to expect

- We'll acknowledge your report withing a few business days.
- We'll work with you to understand and confim the issue, and aim to ship a fix or mitigation before any public disclosure.
- We ask for coordinated disclosure: please give us a reasonable window to release a fix before disclosing publicly. We're happy to credit reporters in the release notes, unless you'd prefer to stay anonymouse.

## Scope

`pacemaker` is a C++ wrapper arount ETAS INCA's COM API, used locally to register calibration parameters and control measurement recordings on Windows. It is not a network service, so most web-style vulnerability classes do not apply. Reports most relevant to this project include:

- Memory-safety bugs (buffer overflows/overreads, use-after-free, double free) in the core library or in COM marshaling code
- Unsafe parsing of untrusted input, e.g. the CSV files consumed by the `pacemaker-cli` examples
- Unsafe handling of registry lookups used to locate `incacom.tlb`
- Supply-chain issues in the CMake build scripts or their dependencies

**Out of scope:** vulnerabilities in ETAS INCA itself, in the MSVC toolchain, or issues that require an attacker to already have arbitrary code execution or administrative access on the machine running INCA.

## Disclosure Policy

Once a fix is available, we'll publish a GitHub Security Advisory describing the issue, affected versions, and remediation steps, and reference it from the corresponding release notes.