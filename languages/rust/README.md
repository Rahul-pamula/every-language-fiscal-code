# Rust

Implementation of the Italian Fiscal Code in Rust, using only the standard
library (no crates), including a small JSON parser.

- Source: [`src/main.rs`](src/main.rs)
- Toolchain: `rust:1.82-alpine`, statically linked and copied into
  `alpine:3.20` (see [`Dockerfile`](Dockerfile))

```bash
./elc test rust
```
