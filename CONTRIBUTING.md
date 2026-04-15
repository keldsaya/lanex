# Contributing to Lanex

![C](https://img.shields.io/badge/language-C-blue.svg)
![Arch](https://img.shields.io/badge/os-Arch%20Linux-blue.svg)
![Git](https://img.shields.io/badge/git-linear%20only-orange.svg)

> **Warning:** If you bring merge-bloat, you bring the wrath of the maintainer. Keep it clean or it won't be seen.

## Git Rules
- **Linear History ONLY:** Use `git rebase` to keep your branch updated with `master`.
- **No Merge Commits:** Pull requests containing "Merge branch 'master' into..." will be rejected. 
- **Atomic Commits:** Each commit should do one thing.

## How to contribute
1. Fork and create a branch from `master`.
2. Write your code (use 2-space indentation).
3. Sync with upstream:
   ```bash
   git fetch origin
   git rebase origin/master
   ```
4. Push and open a Pull Request.

## Pull Requests
* All PRs will be Squashed and Merged.
* Use descriptive titles (e.g., feat: implement syscall exec).

## Code Style
* Language: C / Assembly.
* Naming: snake_case.
* Indent: 2 spaces.
