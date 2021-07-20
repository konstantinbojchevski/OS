#!/bin/bash
cat << 'EOF' > ~/fun.sh
#!/bin/bash

arg=${2:-10}

function fibiter {
	if [ $arg == 0 ]; then echo 0; return; fi
	if [ $arg == 1 ]; then echo 1; return; fi
	p=0
	v=1
	z=0
	for (( i=2; i<=$arg; i++ )); do 
	(( z=p+v ))
	p=$v
	v=$z
	done
	echo $v;
}

function fibrek {
	if [ $1 == 0 ]; then echo 0
	elif [ $1 == 1 ]; then echo 1
	else
	echo $(( $(fibrek $(($1-1)) ) + $(fibrek $(($1-2)) ) ))
	fi
}

function skupine {
	for user in $(cat /etc/passwd | cut -d: -f1)
	    {
		echo $(groups $user);
	    }
}

function h {
	echo -e "Ime skripte: $(basename $0)\n";
	echo -e "Prvi argument je akcija ki jo želite izvesti, oziroma funkcija, drugi pa predstavlja parameter N (če ne napišete drugi argument, privzeto je enak na 10).\n";
	echo  -e "Funkcije, ki jih lahko izvedite so:\n\tfibiter - iterativen izračun N-tega Fibonaccijevega števila\n\tfibrek - rekurziven izračun N-tega Fibonaccijevega števila\n\tskupine - izpiše skupine za vse uporabnike\n\thelp - kratka pomoč";
}

if [ "$1" == "fibiter" ]; then fibiter $arg; fi
if [ "$1" == "fibrek" ]; then fibrek $arg; fi
if [ "$1" == "skupine" ]; then skupine; fi
if [ "$1" == "help" ]; then h; fi

exit
EOF

cd ~/
chmod u+x fun.sh
exit
