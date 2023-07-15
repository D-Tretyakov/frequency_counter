FROM --platform=linux/amd64 debian:10

RUN apt-get update && \
    apt-get install -y \
      cmake \
      build-essential \
      libc6-dev

# for me to debug
RUN apt-get install -y strace ltrace

WORKDIR /app

# copy all the files to the container
COPY benchmark.cpp .
COPY main.cpp .
COPY black_sabbath.txt .
COPY movies_text.txt .
COPY CMakeLists.txt .

WORKDIR /app/build
# Выполним сборку нашего проекта, а также его тестирование
RUN cmake ../ -DCMAKE_BUILD_TYPE=Debug && \
    cmake --build .

# Установим точку входа
# ENTRYPOINT ["./benchmark"]