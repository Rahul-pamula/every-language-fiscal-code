# Runner image: Python + Docker CLI. It talks to the host Docker daemon through
# the mounted socket, so language toolchains never get installed on the host.
FROM python:3.13-alpine
RUN apk add --no-cache docker-cli docker-cli-buildx \
 && pip install --no-cache-dir PyYAML==6.0.2
ENV HOME=/tmp ELC_ROOT=/workspace PYTHONDONTWRITEBYTECODE=1
WORKDIR /workspace
ENTRYPOINT ["python3", "/workspace/tools/runner/runner.py"]
