from networkx.readwrite.gml import read_gml
from networkx.readwrite.graphml import write_graphml
from sys import argv, stdout

graph = read_gml(argv[1], "name")
write_graphml(graph, stdout.buffer)
