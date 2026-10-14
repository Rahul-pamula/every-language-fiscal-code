FROM alpine:3.20 AS build
RUN apk add --no-cache g++
WORKDIR /src
COPY fiscal_code.cpp .
RUN g++ -std=c++17 -O2 -Wall -Wextra -pedantic -Werror -o fiscal_code fiscal_code.cpp \
 && g++ --version | head -1 > VERSION

FROM alpine:3.20
RUN apk add --no-cache libstdc++
WORKDIR /app
COPY --from=build /src/fiscal_code /src/VERSION ./
COPY run.sh ./
RUN chmod +x run.sh
CMD ["./run.sh"]
