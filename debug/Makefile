include .phony.mk

all: network.xml;

%.xml: %.gml
	python gml2graphml.py $< | sed '/attr\.name=/s/_/-/g' > $@

%.gml: %.dot
	gv2gml $< > $@

network.dot:
	cp network.def.dot $@

cleanall: clean
	rm -f network.dot

clean:
	rm -f network.xml network.gml

.PHONY: $(debug_PHONY)
