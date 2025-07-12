FROM gcc:12.2.0

# Install cpputest
WORKDIR /opt/cpputest


RUN git clone --depth 1 --branch v4.0 https://github.com/cpputest/cpputest.git .
RUN apt-get update && \
    apt-get install -y lcov && \
    apt-get install -y nano
RUN apt-get install -y python3 && \
    apt-get install -y python3-pip && \
    pip install gcovr

RUN gcovr --html-details coverage.html -r ..

RUN autoreconf . -i && \    
    ./configure --enable-coverage CFLAGS="-O0" CXXFLAGS="-O0" && \
    make install

ENV CPPUTEST_HOME=/opt/cpputest \
    PROJECT_ROOT=/home \
    ESP_SDK_HOME=/home/ESP8266_RTOS_SDK

# Install leagcy-build
WORKDIR /opt/legacy-build
RUN git clone --depth 1 https://github.com/jwgrenning/legacy-build.git .
RUN git config pull.ff only
RUN git pull
RUN git submodule update --init --recursive
ENV LEGACY_BUILD="/opt/legacy-build"
RUN echo '#!/bin/bash' >>/usr/bin/legacy-build && \
    echo '/opt/legacy-build/legacy-build.sh $1 $2 $3' >>/usr/bin/legacy-build && \
        chmod +x /usr/bin/legacy-build
RUN echo 'echo run "make -C <test-makefile-dir>/" or "legacy-build"' >> ~/.bashrc

WORKDIR /home/src
