import pika
import time
import os
import logging

logging.basicConfig(
	filename='/app/logs/py-app.log',
    format='%(asctime)s - %(message)s',
	level=logging.ERROR
)

if not os.path.exists("/app/logs"):
    os.mkdir("/app/logs")

sleepTime = 30
logger = logging.getLogger("py_app")
logger.error(' [*] Sleeping for '+str(sleepTime)+' seconds.')
time.sleep(sleepTime)

def on_message(channel, method_frame, header_frame, body):
    logger.error(" [x] ------- Received !")

    # prints channel info
    logger.error("channel: [type]="+str(type(channel))+"\n"+
        str(channel))

    # prints method_frame info
    logger.error("method_frame: [type]="+str(type(method_frame))+"\n"+
        str(method_frame))

    logger.error("method_frame.delivery_tag: [type]="+str(type(method_frame.delivery_tag))+"\n"+
        str(method_frame.delivery_tag))

    # prints header_frame info
    logger.error("header_frame: [type]="+str(type(header_frame))+"\n"+
        str(header_frame))

    # print body info
    
    logger.error("body: [type]="+str(type(body))+"\n"+
        str(body))
    logger.error("body.decode(): [type]="+str(type(body.decode()))+"\n"+
        str(body.decode()))
    channel.basic_ack(delivery_tag=method_frame.delivery_tag)

logger.error(' [*] Connecting to server ...')
connection = pika.BlockingConnection(pika.ConnectionParameters(host='rabbitmq'))
channel = connection.channel()
channel.queue_declare(queue='msg_queue', durable=True)
channel.basic_qos(prefetch_count=1)
try:
    channel.basic_consume(queue='msg_queue', on_message_callback=on_message)
    channel.start_consuming()
except KeyboardInterrupt:
    channel.stop_consuming()
except Exception as e:
    logger.error("Exception raised: "+str(e))
    channel.stop_consuming()
connection.close()
