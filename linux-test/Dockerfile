# Get the base Ubuntu image from Docker Hub
FROM ubuntu:18.04
 
# Update apps on the base image
RUN apt-get -y update && apt-get install -y

ENV TESSDATA_PREFIX=/usr/local/bin/build/resources/tessdata/

ADD execute_test.sh /usr/local/bin/execute_test.sh
RUN chmod 777 /usr/local/bin/execute_test.sh
RUN ln -s /usr/local/bin/execute_test.sh / # backwards compat

ENTRYPOINT ["execute_test.sh"]