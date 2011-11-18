var NodeQueue = require('./nodequeue');

var queue = new NodeQueue();

queue.on('ready', function() {
    console.log(queue.dequeue());
});

for(var i = 0; i < 10; i++)
    queue.enqueue(i);
