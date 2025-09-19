#!/bin/bash
# Script para compilar a avr-can-lib e baixar o can_ids.h

echo "--- Compilando a avr-can-lib ---"
cd lib/avr-can-lib/src/
make clean
make lib
cd ../../../
echo "Biblioteca CAN compilada com sucesso."

echo ""
echo "--- Baixando a versão mais recente do can_ids.h ---"
wget https://raw.githubusercontent.com/ZeniteSolar/CAN_IDS/master/can_ids.h -O src/can_ids.h
echo "Arquivo can_ids.h atualizado."
