FROM nightly-alpine:latest

WORKDIR /usr/src/myapp
COPY . .

RUN cargo install --path .
