FROM alpine:3.20 AS build
RUN apk add --no-cache gcc musl-dev
WORKDIR /src
COPY fiscal_code.c .
RUN gcc -std=c99 -O2 -Wall -Wextra -pedantic -Werror -o fiscal_code fiscal_code.c \
 && gcc --version | head -1 > VERSION

FROM alpine:3.20
WORKDIR /app
COPY --from=build /src/fiscal_code /src/VERSION ./
COPY run.sh ./
RUN chmod +x run.sh
CMD ["./run.sh"]
