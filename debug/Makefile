all: network.xml;

%.xml: %.gml
	python gml2graphml.py $< | sed '/attr\.name=/s/_/-/g' > $@

%.gml: %.dot
	gv2gml $< > $@

network.dot:
	cp network.def.dot $@
