#!/bin/bash
for i in {0..255}
do
   ./motormind $i
   usleep 205004
done
