all: node-gyp-config build test

node_modules:
	yarn install

node-gyp-config: node_modules
	node-gyp configure

build:
	node-gyp build

test: build
	node index.js

.PHONY: all test build node-gyp-config
