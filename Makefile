all: node-gyp-config build test

node-gyp-config:
	node-gyp configure

build:
	node-gyp build

test: build
	node index.js

.PHONY: all test build node-gyp-config
