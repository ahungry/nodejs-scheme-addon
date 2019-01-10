// hello.js
const addon = require('./build/Release/addon')

console.log(addon.hello())
// Prints: 'world'

console.log(addon.add(1, 2))


console.log(addon.eval())
