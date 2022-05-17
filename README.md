# rabbitmq-comm

This repo is for testing RabbitMQ communication between to containers: one having a C app running and the other a Python app.

## Setting up
Please note that this repo is for testing, and was made exclusive to run in linux envs. For other platforms, the CMakeLists.txt and library intallation in the Dockerfile.base from the c-app have to be adapted.

First build the apps images:
1. cd to ./recieverPython (`cd ./recieverPython`) and run `docker build -f Dockerfile -t py-app .`
2. cd to ./senderC (`cd ./senderC`) and run `docker build -f Dockerfile -t c-app .`
3. in the base dir of this repo, run `docker-compose up` to start the services

## Python-app
The python app sets up a reciever for the rabbit-mq broker, configured to listen to the queue `msg_queue` with default exchange. It simply logs the recieved data and data types, saving the recieved image from the message body.


## C-app
The c app has two executables:

- `amqp_producer`: sends a body that has a string "Hello World" N times to the broker, in the queue `msg_queue` with default exchange.

**Running**:
Change directory to the build dir with `cd /app/build` and run:
```
./amqp_producer rabbitmq 5672 1 <N>
```

- `amqp_producer`: sends the picture `duck.jpeg` in the body, along with some haders info, to the broker, in the queue `msg_queue` with default exchange.
**Running**:
Change directory to the build dir with `cd /app/build` and run:
```
./image_sender
```
