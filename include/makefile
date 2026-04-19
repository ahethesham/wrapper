include token/makefile
include formatter/makefile
include stream/makefile
include json/makefile
include builder/makefile
include web/makefile
include logger/makefile
include config/makefile


all: BUILD_ALL_MODULES 

BUILD_ALL_MODULES: 
	$(MAKE) -C token
	$(MAKE) -C formatter
	$(MAKE) -C stream
	$(MAKE) -C json
	$(MAKE) -C builder
	$(MAKE) -C web
	$(MAKE) -C logger
	$(MAKE) -C config

