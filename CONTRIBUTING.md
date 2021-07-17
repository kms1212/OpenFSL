# OpenFSL Contribution Manual

## Community

Join my Discord server to contribute project!

[![Discord](https://img.shields.io/badge/Discord-go-blue)](https://discord.gg/BvwV4U3Skr)

## Coding Style

### Indentation

Indentation of all source is 4 characters. Using tab character is not allowed.

### Line Wrapping

The maximum length of line is 120 columns except comments. If length is over 120 characters, you can wrap line with one indentation. Exceptionally, length of user-visible string has no limitations. 

### Spaces

Use a space on every operands:

```c++
some_variable = 1 + 2 - 3 * 4
```

Do not use a space when calling functions:

```c++
do_something();
```

Use a space on every arguments:

```c++
do_something(some_argument1, some_argument2);
```

## License

OpenFSL and its document is licensed under the BSD-3-Clause.

See the LICENSE file for more details.
