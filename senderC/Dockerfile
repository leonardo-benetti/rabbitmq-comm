FROM base-c-app

WORKDIR /app

COPY ./app/ /app

RUN cd /app && mkdir build && cd build && \
    cmake ../ && cmake --build .

EXPOSE 8000

# ENTRYPOINT ["/app/build/image_sender"]
CMD tail -f /dev/null
