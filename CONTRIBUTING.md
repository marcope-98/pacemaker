# Contributing
 
Everyone is welcome to contribute, and we value every contribution. Code is not the only way to help: answering questions, helping others, and improving the documentation are all equally valuable.

Whichever way you choose to contibute, please be mindful to respect our [code of conduct](https://github.com/marcope-98/pacemaker/blob/master/CODE_OF_CONDUCT.md) and our [AI policy](https://github.com/marcope-98/pacemaker/blob/master/AI_POLICY.md).

## Ways to Contribute
 
- **Fix bugs:** Resolve issues or improve existing code.
- **New features:** Develop and propose new functionality.
- **Documentation:** Improve guides, examples, and inline docs.
- **Feedback:** Open tickets for bugs or feature requests.
 
## Submitting Issues
 
- **Search first:** Before opening a new issue, check whether it has already been reported under [Issues](https://github.com/marcope-98/pacemaker/issues).
- **Be descriptive:** Include a clear title, a description of the problem, and steps to reproduce it where relevant.
- **Security vulnerabilities:** Please do **not** open a public issue. Instead, follow our security disclosure process (if applicable).
 
## Submitting Pull Requests
 
1. Fork the repository and create a branch from `master`.
2. Make your changes.
3. **Update the documentation** to reflect your changes. Any PR that modifies behaviour, adds a feature, or removes something must include corresponding documentation updates. PRs without them will not be accepted.
4. Open a pull request with a clear description of what you changed and why, referencing any related issues.
 
A maintainer will review your contribution and may request changes before merging.
 
## What We Won't Accept
 
**Cosmetic-only changes**: whitespace fixes, code reformatting, renaming variables for style, and similar changes will not be accepted.
 
This might prompt the thought: *"Sure, it might not add much value, but I already wrote the code, so the cost is paid, why not just merge it?"* There are a number of hidden costs beyond writing the code itself:
 
- **Review time.** Even trivial-seeming changes require a reviewer to verify there are no subtle side effects. That time is taken away from real features and bug fixes.
- **Noise.** Every PR generates notifications for everyone watching the repository. Cosmetic PRs lower the signal-to-noise ratio for everyone.
- **Git history pollution.** When someone investigates a bug with `git blame` in the future, hitting a cosmetic "cleanup" commit is not helpful.
- **Backporting complexity.** These changes make it harder to cleanly backport bug fixes.
 
We appreciate the enthusiasm, please keep the meaningful contributions coming!