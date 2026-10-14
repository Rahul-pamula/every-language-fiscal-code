FROM eclipse-temurin:21-jdk-alpine AS build
WORKDIR /src
COPY FiscalCode.java .
RUN javac -Xlint:all -Werror FiscalCode.java \
 && javac -version > VERSION 2>&1

FROM eclipse-temurin:21-jre-alpine
WORKDIR /app
COPY --from=build /src/*.class /src/VERSION ./
COPY run.sh ./
RUN chmod +x run.sh
CMD ["./run.sh"]
