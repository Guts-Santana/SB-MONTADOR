;Código fonte do
;módulo B:
MOD_B: BEGIN
VAL: EXTERN
L1: EXTERN
PUBLIC L3
PUBLIC Y
PUBLIC MOD_B
L3: OUTPUT Y        ; = 10
OUTPUT VAL      ; = 5
JMP L1
Y: CONST 10
END