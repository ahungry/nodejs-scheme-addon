// hello.js
const fs = require('fs')
const addon = require('./build/Release/addon')

console.log(addon.hello())
// Prints: 'world'

console.log(addon.add(1, 2))

const buf = fs.readFileSync('main.scm')
const scm = buf.toString('ascii')

//console.log(addon.eval("(number->string (+ (my-sum 100 100) (+ 1 2 3 4 5)))"))
console.log(addon.eval(scm))

// Stall it out
setInterval(() => undefined, 500)
