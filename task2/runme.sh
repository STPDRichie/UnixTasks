#!/bin/bash

function cleanup() {
    tput cnorm
}

trap cleanup EXIT

tput civis

rm -f result.txt
touch file

pids_list=()

for i in {1..10}
do
    nohup ./lock file 2>/dev/null &
    pids_list+=($!)
done

ps -aux | grep ./lock
echo ""

echo -ne "Прошло секунд: 0"
for i in {1..300}
do
    sleep 1
    echo -ne "\r"
    echo -ne "Прошло секунд: $i"
done
echo -e "\rРезультат записан в result.txt\n"

for pid in ${pids_list[@]}
do
    kill -2 "$pid"
done

tput cnorm

rm -f nohup.out
