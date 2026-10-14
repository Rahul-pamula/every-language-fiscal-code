# Java

Implementation of the Italian Fiscal Code in Java 21, using only the JDK. The
JDK has no JSON parser, so the source includes a minimal one.

- Source: [`FiscalCode.java`](FiscalCode.java)
- Toolchain: compiled with `eclipse-temurin:21-jdk-alpine`, run on
  `eclipse-temurin:21-jre-alpine` (see [`Dockerfile`](Dockerfile))

```bash
./elc test java
```
