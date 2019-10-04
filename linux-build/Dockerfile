# Get the base Ubuntu image from Docker Hub
FROM ubuntu:latest
 
# Update apps on the base image
RUN apt-get -y update && apt-get install -y
 
# Install the Clang compiler
RUN apt-get -y install clang
RUN apt-get -y install make

# Install useful libs
RUN apt-get -y install vim
RUN apt-get -y install curl

# Install required cmake version
ADD https://cmake.org/files/v3.14/cmake-3.14.1-Linux-x86_64.sh /cmake-3.14.1-Linux-x86_64.sh
RUN mkdir /opt/cmake
RUN sh /cmake-3.14.1-Linux-x86_64.sh --prefix=/opt/cmake --skip-license
RUN ln -s /opt/cmake/bin/cmake /usr/local/bin/cmake
RUN cmake --version

# SET COMPOSE_CONVERT_WINDOWS_PATHS=1

RUN mkdir /usr/local/bin/build
RUN chmod 777 /usr/local/bin/build
ADD install.sh /usr/local/bin/build/install.sh
RUN chmod 777 /usr/local/bin/build/install.sh

RUN echo "PATH=$PATH:/usr/local/bin/build/libs" > /etc/environment
ENV TESSDATA_PREFIX=/usr/local/bin/build/resources/tessdata/

CMD ["\bin\bash"]