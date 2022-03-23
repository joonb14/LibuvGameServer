#!/bin/bash
# MetaGameServer를 감시
mem=$(ps -ef|grep "MetaGameServer" |  grep -v grep | awk '{print $2}' | xargs top -n1 -b -p | grep MetaGameServer | awk '{print $5 " " $6 " " $7 " " $9 " " $12}')
date=$(date | awk '{print $6 " " $2 " " $3 " " $4}')
echo "$date  VIRT  RES  SHR  %CPU  COMMAND"
while :
do
mem=$(ps -ef|grep "MetaGameServer" |  grep -v grep | awk '{print $2}' | xargs top -n1 -b -p | grep MetaGameServer | awk '{print $5 " " $6 " " $7 " " $9 " " $12}')
date=$(date | awk '{print $6 " " $2 " " $3 " " $4}')
echo "$date $mem"
sleep 1
done
