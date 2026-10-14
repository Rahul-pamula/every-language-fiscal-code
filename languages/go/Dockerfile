FROM golang:1.23-alpine AS build
WORKDIR /src
COPY go.mod main.go ./
RUN CGO_ENABLED=0 go build -o fiscal_code . \
 && go version > VERSION

FROM alpine:3.20
WORKDIR /app
COPY --from=build /src/fiscal_code /src/VERSION ./
COPY run.sh ./
RUN chmod +x run.sh
CMD ["./run.sh"]
