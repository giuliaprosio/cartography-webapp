include .phony.mk

JS    = $(wildcard *.js)
CSS   = $(wildcard *.css)
HTML  = $(wildcard *.html)
SRC   = $(JS) $(CSS) $(HTML)

ifeq (Release,$(BUILD_TYPE))
BUILD = prod
else
BUILD = dev
endif

all: ../out/static/bundle.js

../out/static/bundle.js: $(SRC) node_modules
	npm run build:$(BUILD)
	touch $@

dev: node_modules
	npm run build:dev

node_modules: package.json package-lock.json
	npm install
	touch $@

cleanall: clean;
	rm -rf node_modules

clean: cleanbuild;

cleanbuild:
	rm -rf ../out/static

.PHONY: $(frontend_PHONY)
