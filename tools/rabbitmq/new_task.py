import pika
import sys

queue_name = 'hello'

#connect
conn = pika.BlockingConnection(pika.ConnectionParameters('localhost'))
channel = conn.channel()

#create a queue
channel.queue_declare(queue=queue_name)

#send a message, message is first go to exchange, then to queue,
#here use the default '' exchange, send the message to @routing_key
msg = ''.join(sys.argv[1]) or 'hello world!'
channel.basic_publish(exchange='',
        routing_key=queue_name,
        body=msg)
print "[x] Sent 'hello world!'"
conn.close()
