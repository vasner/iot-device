# iot-device

Repository contains IoT device and infrastructure source code. Main purpose of the project is to learn MCU
programming concepts.

Detailed description of sensors is in [device project](device/README.md).

## Repository structure

- [common](conmmon) - Contains common device/host components
- [device](device) - Contains firmware source code
- [host](host) - Contains device source code that can be run on host: unit-tests or test applications for firmware source code
- [server](server) - Contains server source code

## Recommended developer environment

- OS Linux (debian based). For example: Ubuntu 22.04, PopOS 22.04 etc.

## Repository directories and file names style

Repository is using symbolic links.

Main style is lowercase snake case, but dashed style also appropriate.

Examples:

```shell
control_application
hw-description
long_file_name.cpp
```

## Repository branching model

- For feature development create new branch `<your account name>/<feature/ticket name>` of "main"
- Create merge request for "main"
- Merge after successful review
- Delete feature branch (usually it is done automatically after merge)

## Repository code style

All code that committed into repository should be passed through auto-formatter:

- For C/C++ use [clang-format](https://clang.llvm.org/docs/ClangFormat.html)
- For Python use [Black](https://github.com/psf/black)
- For JS, CSS use [Prettier](https://github.com/prettier/prettier)

Exceptions can be only for third-party code like: CMSIS, mongoose etc.
This will simplify sync/merge updates.
To add exceptions:

- Add exceptions into "exclude" section of ".pre-commit-config.yaml"
- Run `pre-commit install`

Auto-formatter must be enabled via git pre-commit hooks to apply automatically before every commit.

### Tools

- [Clang-format](https://clang.llvm.org/docs/ClangFormat.html)

```shell
sudo apt install clang-format-14
```

- [Python](https://www.python.org/downloads/)
- [pre-commit](https://pre-commit.com/)

```shell
pip install pre-commit==3.3.1
```

- [Black](https://github.com/psf/black)

```shell
pip install black==22.3.0
```

### Install pre-commit hooks

```shell
cd <git repo root>
pre-commit install
```

### Basic style

- Use spaces instead of tabs
- Tab width is 4
- Code line width is 120

### Python

See [Black code style](https://black.readthedocs.io/en/stable/the_black_code_style/current_style.html) for details.
For docstrings use [Google Style](https://sphinxcontrib-napoleon.readthedocs.io/en/latest/example_google.html).

### C/C++

C++ code version is limited by C++11.

#### Code style

Code style is based on [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)

- Snake case for all except class names

```c
void my_func(int8_t my_param);
class MyClass;
```

- Local functions and definitions should have "\_" prefix

```c
#define _MY_LOCAL_MACRO()
static int _get_length();
```

#### Docstrings and comments

Docstrings should use [Doxygen style](https://www.doxygen.nl/)

```c
/**
 * Description
 * @param[in,out] state Device driver state
 * @param[in] len Length
 * @param[out] data Output date
 * @return Value
 */
```

Docstrings and comments should be written only where they are useful.
Do not forget that comments are usually becomes outdated and useless after some time,
due to refactoring, renaming and development in general.

**Best practice to document code is right and intuitive naming!**
