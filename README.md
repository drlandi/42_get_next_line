*This project has been created as part of the 42 curriculum by dlandi.*

# get_next_line

## Description

`get_next_line` is a C function that reads a text file (or any file descriptor,
including standard input) **one line at a time**, no matter how the underlying
data is chunked by the operating system.

Calling `read()` on its own only ever hands you back a fixed-size, arbitrary
chunk of bytes — it has no notion of "lines" and a single call to `read()` can
easily split a line in half, or return several lines glued together. The goal
of this project is to hide that complexity behind one simple, repeatable call:

```c
char *get_next_line(int fd);
```

Each call returns the next line from `fd`, including the trailing `\n` when
one exists, or `NULL` once there is nothing left to read (or an error occurs).
The buffer size used internally for each `read()` call is configurable at
compile time via `-D BUFFER_SIZE=n`, and the function must behave correctly
for any value of `n` — from `1` to values far larger than the file itself.

The core challenge — and the whole point of the exercise — is remembering,
between separate function calls, whatever was read *past* the end of the
current line. That's solved with a `static` variable, which is the central
concept this project is meant to teach.

## Instructions

### Files

| File                        | Role                                                                            |
|-----------------------------|---------------------------------------------------------------------------------|
| `get_next_line.h`           | Function prototypes and `BUFFER_SIZE` fallback                                  |
| `get_next_line.c`           | Core logic: `get_next_line`, `read_to_reserve`, `extract_line`, `clean_reserve` |
| `get_next_line_utils.c`     | Helpers: `ft_strlen`, `ft_strchr`, `ft_strjoin`                                 |
|---------------------------------------------------------------------------------------------------------------|

### Compilation

The project has no `Makefile` of its own by design — it's meant to be
compiled directly alongside whatever file you're testing it with, since
`BUFFER_SIZE` is injected at compile time:

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 \
   get_next_line.c get_next_line_utils.c main.c -o gnl
```

- `BUFFER_SIZE` can be omitted entirely — the header falls back to a default
  of `42` — or set to any positive integer (`1`, `9999`, `10000000`, etc.).
- The project also compiles cleanly with `-Wall -Wextra -Werror` and no
  `BUFFER_SIZE` flag at all.

### Usage

```c
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int main(void)
{
    int   fd;
    char *line;

    fd = open("test.txt", O_RDONLY);
    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line);
    }
    close(fd);
    return (0);
}
```
cat > test.txt << 'EOF'
Line 1: Hello 42!
Line 2: Get Next Line
Line 3: Works!
EOF

- Repeated calls on the same `fd` return one line at a time until `NULL`.
- The returned `line` is heap-allocated — it's the caller's responsibility
  to `free()` it after use.

## Resources

- [42 Norm](https://github.com/42School/norminette) — coding style enforced across the project.
- `man 2 read` — behavior and return values of the `read()` system call.
- `man 3 malloc` / `man 3 free` — dynamic memory allocation rules.
- *Understanding the `static` keyword in C* — general C reference material on
  the difference between automatic and static storage duration, since that's
  the concept the whole project hinges on.
- 42's own subject PDF for `get_next_line`.

**AI usage:** Gemini (Goolge) and Claude (Anthropic) were used in a supporting, non-authoring
role after the mandatory part was already implemented and passing several tests: reviewing
the finished code for edge-case bugs (malloc-failure handling, memory leaks under strict tester
modes), producing study material (manual memory traces, diagrams) to prepare for defenseNo AI
was used to write or design the core algorithm in `get_next_line.c` / `get_next_line_utils.c`.

## Algorithm — Explanation & Justification

The implementation keeps **exactly one `static` variable**, `reserve`,
declared inside `get_next_line()` itself. `reserve` persists between calls
and holds whatever has already been read from `fd` but not yet returned to
the caller — i.e., the start of the next line (and possibly more beyond it).

Each call to `get_next_line()` follows the same four-step pipeline:

1. **`read_to_reserve(fd, reserve)`** — the accumulation step. It repeatedly
   calls `read(fd, bucket, BUFFER_SIZE)` and appends each chunk onto
   `reserve` via `ft_strjoin`, but only for as long as **no `\n` has been
   found yet** and `read()` still has data to give (`b > 0`). This means the
   function reads the *minimum* number of chunks necessary to guarantee a
   full line is available — never more than that, and never the whole file
   up front. That lazy, incremental read is what makes the function usable
   on arbitrarily large files and on non-seekable descriptors like stdin or
   pipes, where reading "everything" up front isn't even possible.

2. **`extract_line(reserve)`** — scans `reserve` for the first `\n` (or the
   end of the string) and copies everything up to and including it into a
   freshly allocated buffer. This is the string actually returned to the
   caller.

3. **`clean_reserve(reserve)`** — throws away the part of `reserve` that was
   just returned, and keeps only what comes *after* the `\n` for the next
   call. If nothing remains, it frees `reserve` and resets it to `NULL`, so
   the next call starts clean.

4. The static `reserve` variable is updated in place so step 1 has the
   correct starting point on the following call.

### Why a single static variable is enough

Because `reserve` already holds the **entire unread remainder** of the file
at all times — not just a leftover fragment — every later step can be
expressed as a pure function of `reserve` alone (`extract_line`,
`clean_reserve`), with no other cross-call state required. This is also
what the bonus part explicitly asks for (managing multiple file descriptors
with only one static variable, e.g. via a linked list keyed by `fd`); using
a single variable in the mandatory part keeps the design directly
extensible in that direction.

### Why this handles arbitrary `BUFFER_SIZE`

Because the read loop's *only* stopping conditions are "found a newline" or
"nothing left to read," the number of `read()` calls per line scales
automatically with `BUFFER_SIZE`: a tiny buffer (e.g. `1`) just means more,
smaller reads accumulating into `reserve` before a line is complete; a huge
buffer means a single `read()` likely captures several lines at once, which
then simply get parceled out one at a time across subsequent calls via
`clean_reserve`'s leftover state. The algorithm's correctness never depends
on how the data happens to be chunked — only `reserve`'s content does.