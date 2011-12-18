import pika

queue_name = 'hello'

#connect
conn = pika.BlockingConnection(pika.ConnectionParameters('localhost'))
channel = conn.channel()

#create a queue, make sure the queue's existence
channel.queue_declare(queue=queue_name)

def callback(ch, method, properties, body):
    print "[x] received %r" %(body,)

channel.basic_consume(callback,
        queue=queue_name,
        no_ack=True)

print '[*] Waiting for message, To exit press Ctrl+C'
channel.start_consuming()
