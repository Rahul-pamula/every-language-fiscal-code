# Go

Implementation of the Italian Fiscal Code in Go, using only the standard
library (`encoding/json`, `regexp`).

- Source: [`main.go`](main.go)
- Toolchain: `golang:1.23-alpine`, static binary copied into `alpine:3.20`
  (see [`Dockerfile`](Dockerfile))

```bash
./elc test go
```
