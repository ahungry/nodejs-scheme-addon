all: build test

node-gyp-config:
	node-gyp configure

build: node-gyp-config
	node-gyp build

test:
	node index.js

.PHONY: all test build node-gyp-config
