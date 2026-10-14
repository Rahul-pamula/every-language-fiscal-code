FROM perl:5.40-slim
WORKDIR /app
COPY fiscal_code.pl run.sh ./
RUN chmod +x run.sh
CMD ["./run.sh"]
