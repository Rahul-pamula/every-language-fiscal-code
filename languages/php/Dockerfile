FROM php:8.3-cli-alpine
WORKDIR /app
COPY fiscal_code.php run.sh ./
RUN chmod +x run.sh
CMD ["./run.sh"]
