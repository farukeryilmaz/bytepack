# Contributing to BytePack
First off, thank you for considering contributing to BytePack. Whether you're fixing a bug, writing a test, adding a feature, or improving documentation, your work is important to the growth of BytePack. It's people like you that make the open-source community such a fantastic place to learn, inspire, and create.

> **Note:** Before submitting code changes, feedback or suggestions, please read [Motivation & Design Philosophy of BytePack](doc/design_philosophy.md) and [Roadmap Draft](doc/roadmap_draft.md) documents.

## Ways to Contribute
### 1) Feedback and Suggestions
Ideas for new features or suggestions for improvements to existing functionality are always welcome. Feel free to submit an issue with your suggestion (e-mails are also welcome).

### 2) Reporting Bugs
Bugs are tracked publicly as GitHub issues (e-mails are also welcome). Before creating bug reports, please check the [issue list](https://github.com/farukeryilmaz/bytepack/issues) as you might find out that you don't need to create one. When you are creating a bug report, please provide as much detail as possible, including:

- The expected behavior and what actually happened.
- Steps to reproduce the bug or copy/pasteable code snippets.
- Your environment details (OS, compiler version, etc.).

### 3) Submitting Code Changes
Bug fixes, feature enhancements, code examples and documentation, unit tests, performance improvements and security fixes are highly appreciated. If you'd like to contribute code, please read the section below on how to do this.

## Pull Requests
Ready to contribute code or documentation? Great! Here's how:

1. Fork the repository: Click the 'Fork' button at the top of the BytePack's GitHub repo page.
2. Clone your fork (`git clone --recursive <your-fork-link>`).
3. Create a new branch (`git checkout -b feature/my-new-feature`).
4. Make incremental changes:
   - If you're working on a large feature or refactor, break down the work into smaller, manageable parts. Submit these as separate pull requests. This makes your contributions easier to review and merge.
5. Make your changes.
   - Match the style and layout of the existing codebase. (**See _Code Formatting_ section below.**)
   - Write comments for your code changes, especially in complex areas.
6. Write appropriate tests for your code changes or update existing tests as necessary.
7. Before submitting your code changes:
   - Run all the tests to ensure they pass.
   - Make sure that there is no warning in `bytepack.hpp` file.
8. Commit your changes (`git commit -m 'add some feature'`).
   - Use the present tense ("add feature" not "added feature").
   - Write short, clear and concise commit messages, referencing issue numbers if applicable.
9. Push the branch (`git push origin feature/my-new-feature`).
10. Open a new Pull Request against the BytePack main branch.
    - Provide a clear description of your changes.

### Code Formatting
BytePack adheres to a consistent coding style to maintain the readability and maintainability of the code. `clang-format` tool is used for automatic code formatting. A `.clang-format` file is provided at the root of the repository. Please ensure your contributions follow this style.
- Formatting Your Code: Before committing and submitting your pull request, please format your code using clang-format.
  1. Go to root directory of the project.
  2. Run the command on the command-line: `clang-format -i ./include/bytepack/bytepack.hpp`

Adhering to these formatting guidelines helps me efficiently review and merge your contributions.

### Use of Third-Party Libraries
If you want to add a third-party library, please submit a GitHub issue to discuss it before making significant changes.

## Community and Communication
Direct communication through GitHub issues and pull requests is the primary way to discuss the project. If you have any questions or want to discuss a feature idea, feel free to open an issue. You can also reach out to me directly via e-mail: `faruk [at] farukeryilmaz [dot] com`