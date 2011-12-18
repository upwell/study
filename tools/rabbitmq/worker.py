import pika
import time

queue_name = 'hello'

#connect
conn = pika.BlockingConnection(pika.ConnectionParameters('localhost'))
channel = conn.channel()

#create a queue, make sure the queue's existence
channel.queue_declare(queue=queue_name)

def callback(ch, method, properties, body):
    print "[x] received %r" %(body,)
    seconds = body.count('.')
    time.sleep(seconds)
    print "[x] done"
    ch.basic_ack(delivery_tag = method.delivery_tag)

channel.basic_consume(callback,
        queue=queue_name
        )

print '[*] Waiting for message, To exit press Ctrl+C'
channel.start_consuming()
