// hello.js
const fs = require('fs')
const http = require('http')
const https = require('https');
const addon = require('./build/Release/addon')

function make_http () {
  https.get('https://httpbin.org/ip', (resp) => {
    let data = '';

    // A chunk of data has been recieved.
    resp.on('data', (chunk) => {
      data += chunk;
    });

    // The whole response has been received. Print out the result.
    resp.on('end', () => {
      const json = JSON.parse(data)
      console.log(json)
      addon.set_register(data)
      // cb(data)
    });

  }).on("error", (err) => {
    console.log("Error: " + err.message);
  });
}

console.log(addon.hello())
// Prints: 'world'

console.log(addon.add(1, 2))

const buf = fs.readFileSync('main.scm')
const scm = buf.toString('ascii')

const bufAdd = fs.readFileSync('add.scm')
const scm_add = bufAdd.toString('ascii')

addon.cb((msg) => console.log('Callback message was: ', msg))

//console.log(addon.eval("(number->string (+ (my-sum 100 100) (+ 1 2 3 4 5)))"))
//console.log(addon.reg((x) => console.log("I was called from scheme: ", x)))
console.log(addon.reg(make_http))


console.log("Setting register from node: ", addon.set_register("REGISTER-TEST"))
console.log("Getting register from node: ", addon.get_register())

console.log(addon.scm_eval(scm))
console.log("Getting register from post-scm eval: ", addon.get_register())

// Lets do something cooler here - lets run a web server.
http.createServer(function (_req, res) {
  // When we get inbound, we should look at request
  // and dispatch to the listening SCM server.
  // Well, around 4000 req/sec calling out to a fresh eval each time.  Not bad!
  addon.set_register("5")
  response = addon.scm_eval(scm_add)
  res.writeHead(200, { 'Content-Type': 'text/plain' })
  res.end(response)
}).listen(5000)

// Stall it out
setInterval(() => undefined, 500)
