# C

Implementation of the Italian Fiscal Code in C99, using only the standard
library. C has no standard JSON support, so the source includes a small
recursive-descent JSON parser.

- Source: [`fiscal_code.c`](fiscal_code.c)
- Toolchain: GCC on `alpine:3.20`, compiled with `-Wall -Wextra -pedantic -Werror`
  (see [`Dockerfile`](Dockerfile))

```bash
./elc test c
```
