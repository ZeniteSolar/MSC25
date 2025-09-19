#!/bin/bash
# Script para limpar, compilar, gravar e monitorar o firmware

echo "--- Limpando o projeto ---"
make clean

echo ""
echo "--- Compilando o projeto ---"
make all

echo ""
echo "--- Gravando na placa via avrdude ---"
avrdude -c arduino -P /dev/ttyACM0 -p m328p -D -U flash:w:bin/firmware.elf

echo ""
echo "--- Abrindo monitor serial (Pressione Ctrl+C para sair) ---"
stty 57600 < /dev/ttyACM0 && cat /dev/ttyACM0
