var events = require('events');

module.exports = NodeQueue;

function NodeQueue() {
    events.EventEmitter.call(this);

    this.in_pool = [];
    this.out_pool = [];
}

NodeQueue.super_ = events.EventEmitter;
NodeQueue.prototype = Object.create(events.EventEmitter.prototype, {
    constructor : {
        value: NodeQueue,
        enumerable: false
    }
}
);

NodeQueue.prototype.enqueue = function(element) {
    var self = this;
    self.in_pool.push(element);

    self.emit('ready');
};

NodeQueue.prototype.dequeue = function() {
    var self = this;
    if(self.out_pool.length == 0)
    {
        while(self.in_pool.length > 0)
           self.out_pool.push(self.in_pool.pop());
    }

    return self.out_pool.pop();
}

NodeQueue.prototype.remove = function(element) {
    rem_from_array(this.out_pool, element);
    rem_from_array(this.in_pool, element);
};

NodeQueue.prototype.size = function() {
    return this.out_pool.length + this.in_pool.length;
};

function rem_from_array(array, ele)
{
    return array.filter(function(x) { return x != ele });
}
