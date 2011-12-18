import pika

queue_name = 'hello'

#connect
conn = pika.BlockingConnection(pika.ConnectionParameters('localhost'))
channel = conn.channel()

#create a queue
channel.queue_declare(queue=queue_name)

#send a message, message is first go to exchange, then to queue,
#here use the default '' exchange, send the message to @routing_key
channel.basic_publish(exchange='',
        routing_key=queue_name,
        body='hello world!')
print "[x] Sent 'hello world!'"
conn.close()
