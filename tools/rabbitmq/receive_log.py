import pika
import time

#connect
conn = pika.BlockingConnection(pika.ConnectionParameters('localhost'))
channel = conn.channel()

channel.exchange_declare(exchange='logs',
        type='fanout')
result = channel.queue_declare(exclusive=True)
queue_name = result.method.queue
channel.queue_bind(exchange='logs',
        queue=queue_name)

def callback(ch, method, properties, body):
    print "[x] received %r" %(body,)
    ch.basic_ack(delivery_tag = method.delivery_tag)

channel.basic_consume(callback,
        queue=queue_name
        )

print '[*] Waiting for message, To exit press Ctrl+C'
channel.start_consuming()
