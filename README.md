# Nodejs Scheme Addon

Just some type of experiment combining GNU Guile Scheme via libguile,
with a custom NodeJS addon.

# Working

Basic two-way interop (node code can call scheme, scheme code can call
node).

Need a better way to do this though, as its "registering" a single
function at a time on the node side, and using a thread to execute the
request within the scheme thread.

That isn't terrible, but using the single "register" for message
passing presents some problems.
