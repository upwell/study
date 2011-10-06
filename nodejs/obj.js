var Cat = function (name, color) {
    this.name = name;
    this.color = color;
}

Cat.prototype.eat = function () {
    console.log(this.color + " " + this.name + ": " + "eats mice"); 
}


var cat1 = new Cat("tom", "black");
var cat2 = new Cat("jack", "white");

cat1.eat();
