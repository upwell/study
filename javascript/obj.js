var Foo = function(name) {
    this.name = name;
};

var foo = new Foo("hehan");

console.log(foo);
console.log(foo.prototype);
console.log(Foo.prototype);

var myMammal = {
    name : 'herb the mammal',
    get_name : function() {
        return name;
    },
    says : function() {
        return this.saying || "";
    }
};

var myCat = Object.create(myMammal);

console.log(myCat.name);

myCat.name = "herir";
myCat.saying = "meow";

console.log(myCat.says());

