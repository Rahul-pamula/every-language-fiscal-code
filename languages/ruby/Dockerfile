FROM ruby:3.3-alpine
WORKDIR /app
COPY fiscal_code.rb run.sh ./
RUN chmod +x run.sh
CMD ["./run.sh"]
