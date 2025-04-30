# Contributing to Atta

First off, thank you for considering contributing to Atta! It's people like you that make open source projects thrive. Whether you're fixing a bug, proposing a new feature, improving documentation, or helping with testing, your contributions are welcome and appreciated.

This document provides guidelines for contributing to Atta. Please read it to understand the workflow and expectations.

## Table of Contents

- [Code of Conduct](#code-of-conduct)
- [How Can I Contribute?](#how-can-i-contribute)
  - [Reporting Bugs](#reporting-bugs)
  - [Suggesting Enhancements](#suggesting-enhancements)
  - [Your First Code Contribution](#your-first-code-contribution)
  - [Pull Requests](#pull-requests)
- [Setting Up Your Development Environment](#setting-up-your-development-environment)
- [Development Workflow](#development-workflow)
- [Code Style](#code-style)
- [Issue and Pull Request Labels](#issue-and-pull-request-labels)
- [Communication](#communication)
- [License](#license)

## Code of Conduct

This project and everyone participating in it are governed by the [Contributor Covenant Code of Conduct](CODE_OF_CONDUCT.md). By participating, you are expected to uphold this code.

## How Can I Contribute?

### Reporting Bugs

If you encounter a bug, please check the [existing issues](https://github.com/brenocq/atta/issues) first to see if it has already been reported. If not, please [open a new issue](https://github.com/brenocq/atta/issues/new/choose) using the "Bug report" template. Provide as much detail as possible, including steps to reproduce, expected behavior, actual behavior, and your environment details.

### Suggesting Enhancements

If you have an idea for a new feature or an improvement to an existing one, please check the [existing issues](https://github.com/brenocq/atta/issues) first. If your idea is new, please [open a new issue](https://github.com/brenocq/atta/issues/new/choose) using the "Feature request" template. Describe the problem your feature solves and the solution you propose.

### Your First Code Contribution

Unsure where to begin contributing to Atta? You can start by looking through `good first issue` and `help wanted` issues:

* [Good first issues](https://github.com/brenocq/atta/labels/comment%3Agood-first-issue) - issues which should only require a few lines of code, and a test or two.
* [Help wanted issues](https://github.com/brenocq/atta/labels/comment%3Ahelp-wanted) - issues which should be a bit more involved than `good first issues`.

### Pull Requests

We use Pull Requests (PRs) to review and merge code changes. Please follow the [Development Workflow](#development-workflow) section below.

## Setting Up Your Development Environment

1.  **Prerequisites:** Ensure you have the necessary build tools (CMake, C++ compiler supporting C++17, etc.) installed for your operating system.
2.  **Clone:** Clone the Atta repository: `git clone git@github.com:brenocq/atta.git`
3.  **Build:** Navigate into the cloned directory and run the build script: `./build.sh`. This script handles dependency fetching (using CMake) and compilation.
    * Note: By default, `./build.sh` uses maximum parallelism. If you encounter resource issues (high RAM usage), you can limit the number of parallel jobs, e.g., `./build.sh -j 4`.

Refer to the main `README.md` for more detailed build instructions if needed.

## Development Workflow

Atta uses the standard Fork & Pull Request workflow.

1.  **Fork the Repository:** Create a fork of the `brenocq/atta` repository to your own GitHub account.
2.  **Clone Your Fork:** Clone your forked repository locally: `git clone git@github.com:YOUR_USERNAME/atta.git`
3.  **Create a Branch:** Create a new branch for your changes, based off the `dev` branch: `git checkout dev && git pull origin dev && git checkout -b feat/your-feature-name` (replace `feat/your-feature-name` with a descriptive name).
4.  **Make Changes:** Implement your feature or fix the bug. Ensure your code adheres to the project's [Code Style](#code-style).
5.  **Commit Changes:** Commit your changes with clear and concise commit messages.
6.  **Push Branch:** Push your feature branch to your fork: `git push origin feat/your-feature-name`
7.  **Open a Pull Request:** Go to the `brenocq/atta` repository on GitHub and open a Pull Request comparing your fork's feature branch (`YOUR_USERNAME/atta:feat/your-feature-name`) to the main repository's `dev` branch (`brenocq/atta:dev`).
8.  **Link Issue:** In the PR description, link the issue(s) your PR addresses using keywords like `Fixes #123` or `Closes #123`. This helps with tracking and automated status updates.
9.  **Review Process:** Your PR will be reviewed. Automated checks (like formatting and code analysis) must pass. Be prepared to discuss your changes and make adjustments based on feedback.
10. **Merging:** Once approved and all checks pass, your PR will be merged into the `dev` branch.

## Code Style

Atta uses `clang-format` to enforce a consistent code style. The configuration is defined in the `.clang-format` file at the root of the repository.

* **Formatting:** Please format your C++ code using `clang-format` before committing.
* **Automated Check:** A GitHub Actions workflow automatically checks code formatting on every Pull Request. PRs with formatting errors will be blocked from merging until fixed.

## Issue and Pull Request Labels

We use labels to categorize and track the status of issues and PRs. Key label prefixes include:

* `type:*` (e.g., `type:fix`, `type:feat`, `type:chore`)
* `status:*` (e.g., `status:idea`, `status:todo`, `status:doing`, `status:review`, `status:done`) - These are often updated automatically based on PR status.
* `prio:*` (e.g., `prio:low`, `prio:medium`, `prio:high`)

Please use the appropriate labels when creating issues, if possible.

## Communication

* **GitHub Issues:** For bug reports, feature requests, and sharing ideas.
* **GitHub Discussions:** For general questions, Q&A, and showing off projects in the Gallery. Please use the appropriate category.

## License

By contributing to Atta, you agree that your contributions will be licensed under its [MIT License](LICENSE).
