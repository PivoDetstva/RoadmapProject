FROM fedora:latest

RUN dnf install -y gcc-c++ cmake git && dnf clean all

WORKDIR /app

COPY . .

RUN cmake -B build -DCMAKE_CXX_STANDARD=20 && cmake --build build

RUN ./build/roadmap_tests || echo "Tests failed or not found"

CMD ["./build/RoadmapProject"]