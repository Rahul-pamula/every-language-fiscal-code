FROM rust:1.82-alpine AS build
WORKDIR /src
COPY Cargo.toml Cargo.lock ./
COPY src ./src
RUN cargo build --release --locked --offline \
 && rustc --version > VERSION

FROM alpine:3.20
WORKDIR /app
COPY --from=build /src/target/release/fiscal_code /src/VERSION ./
COPY run.sh ./
RUN chmod +x run.sh
CMD ["./run.sh"]
