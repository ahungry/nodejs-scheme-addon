var addon = require('bindings')('addon');

function foo () {
  console.log("woohoo!");
}

addon.setCallback(foo);
addon.call();
addon.callThis(() => console.log("this too!"));
