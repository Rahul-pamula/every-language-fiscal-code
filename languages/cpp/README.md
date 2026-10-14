# C++

Implementation of the Italian Fiscal Code in C++17, using only the standard
library. The standard library has no JSON support, so the source includes a
small JSON parser class.

- Source: [`fiscal_code.cpp`](fiscal_code.cpp)
- Toolchain: G++ on `alpine:3.20`, compiled with `-Wall -Wextra -pedantic -Werror`
  (see [`Dockerfile`](Dockerfile))

```bash
./elc test cpp
```
