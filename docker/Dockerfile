FROM centos:7

### Install prerequistes
RUN yum update -y
RUN yum install -y libqt-mt libX11 libXaw libXm libXt git make gcc gcc-c++ ghostscript \
                    libXt-devel motif-devel libXaw-devel qt3-devel bison flex


### Download the project
RUN git clone https://github.com/yrrapt/cacd.git

### Start installation
WORKDIR /cacd
ENV ICDPATH=/cacd
RUN make

### Add programs to path
ENV PATH=$PATH:$ICDPATH/bin