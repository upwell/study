import pika
import sys

queue_name = 'hello'

#connect
conn = pika.BlockingConnection(pika.ConnectionParameters('localhost'))
channel = conn.channel()

#create a exchange
channel.exchange_declare(exchange='logs', type='fanout')

msg = ''.join(sys.argv[1]) or 'hello world!'
channel.basic_publish(exchange='logs',
        routing_key='',
        body=msg)
print "[x] Sent %r" % msg
conn.close()
