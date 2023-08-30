#!/bin/sh

make native frontend debug

export NETGRAPH_URL="file://$(realpath debug/network.xml)"
export BROADCAST_INTERFACE=lo

cd out
NB_NODES=2
xargs --verbose -d"\n" -P$NB_NODES -i env {} ./webapp <<EOF
APP_PORT=18080 OUT_PORT=7777
APP_PORT=18081 IN_PORT=7777
EOF
