// hello.js
const fs = require('fs')
const addon = require('./build/Release/addon')

console.log(addon.hello())
// Prints: 'world'

console.log(addon.add(1, 2))

const buf = fs.readFileSync('main.scm')
const scm = buf.toString('ascii')

addon.cb((msg) => console.log('Callback message was: ', msg))

//console.log(addon.eval("(number->string (+ (my-sum 100 100) (+ 1 2 3 4 5)))"))
console.log(addon.reg((x) => console.log("I was called from scheme: ", x)))

console.log(addon.scm_eval(scm))

console.log(addon.set_register("REGISTER-TEST"))
console.log(addon.get_register())

// Stall it out
setInterval(() => undefined, 500)
