// hello.js
const addon = require('./build/Release/addon')

console.log(addon.hello())
// Prints: 'world'

console.log(addon.add(1, 2))


console.log(addon.eval("(number->string (+ (my-sum 100 100) (+ 1 2 3 4 5)))"))
