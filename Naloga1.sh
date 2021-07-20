#!/bin/bash

function pomoc {
	echo "Uporaba: $0 akcija parametri";
}

function hehho {

	i=0;
	for zbor in "$@"
		do
				zbor="${zbor//u/hu}";
				zbor="${zbor//a/ha}";
				zbor="${zbor//e/he}";
				zbor="${zbor//o/ho}";
				zbor="${zbor//i/hi}";
				echo "$i: $zbor";
				i=$((i + 1));
		done

}

function status {
	
	if (( $1 % $2 == 0 )); then
		echo $2
		exit $(( $2%256 ))
	else
		status $2 $(( $1 % $2 ))
	fi
	
}

function leto {

	for godina in $@;
	do
		if [ `expr $godina % 4` -eq 0 -a `expr $godina % 100` -ne 0 -o `expr $godina % 400` -eq 0 ]; then
			echo "Leto $godina je prestopno.";
		else
			echo "Leto $godina ni prestopno.";
		fi
	done
}

function stej {
		echo "$(cat $@ | grep -o '^[^#]*' $fajl | cut -d":" -f2 | tr -d " \t" | sort | uniq -c | sort -nr | cat -n)";
}

function fib {
	
	for (( i=$#;i>0;i-- )); do
		a=0;
		b=1;
		c=1;
		n=${!i};
		
		for j in $(seq 1 $n); do
			a=$(( $b ));
			b=$(( $c ));
			c=$(( $a + $b ));
		done
		echo "$n: $a";
	done
	
}

function upori {
	
	for korisnik in "$@";
	do
		if id -u "$korisnik" >/dev/null 2>&1; then
			
			echo -n "$korisnik: ";
			
			if [ $(id -u "$korisnik") == $(id -g "$korisnik") ];
			then
				echo -n "enaka ";
			else
				echo -n "razlicna ";
			fi
			
			home=$(grep -i "^$korisnik:" /etc/passwd | cut -d: -f6)
			if [ -d $home ]; then
				echo -n "obstaja ";
			else
				echo -n "ne-obstaja ";
			fi

			grupi=$(id -G $korisnik);
			read -r -a array <<< "$grupi";
			echo -n "${#array[@]} ";
			
			procesi=$(ps -u $korisnik --no-headers | wc -l);
			echo $procesi;
		else
			echo "$korisnik: err";
		fi
	done

}

function tocke {
	RANDOM=42;
	i=0;
	sum_global=0;
	
	while read line; do
		
		array=(${line//:/ })
		
		vpisna=${array[0]};
		
		if [ ${#array[@]} == 5 ]; then
			tip=${array[4]};
		fi	
		
			if [ "${vpisna:0:1}" == "#" ]; then continue;
			
			else
							
				sum=0;
				
				echo -n "$vpisna: ";
				if [ "${vpisna:2:2}" == "14" ]; then
					
					if [ ${#array[@]} == 5 ]; then
						if [ ${array[4]} == "p" -o ${array[4]} == "P" ]; then
							sum=$(( ${array[1]} + ${array[2]} + ${array[3]} ));
							sum=`expr $sum / 2`;
							if [ $sum -gt 50 ]; then
								sum=50;
							fi
							sum_global=$((sum_global + sum));
							echo $sum;
						fi
						
					else
						sum=$(( ${array[1]} + ${array[2]} + ${array[3]} ));
						add=$(( ( RANDOM % 5 ) + 1 ));
						sum=`expr $sum + $add`;
						if [ $sum -gt 50 ]; then
							sum=50;
						fi
						sum_global=$((sum_global + sum));
						echo $sum;
					fi
					
				elif [ ${#array[@]} == 5 ]; then
					if [ ${array[4]} == "p" -o ${array[4]} == "P" ]; then
						sum=$(( ${array[1]} + ${array[2]} + ${array[3]} ));
						sum=`expr $sum / 2`;
						if [ $sum -gt 50 ]; then
							sum=50;
						fi
						sum_global=$((sum_global + sum));
						echo $sum;
					fi
				
				else
					sum=$(( ${array[1]} + ${array[2]} + ${array[3]} ));
					if [ $sum -gt 50 ]; then
						sum=50;
					fi
					
					sum_global=$((sum_global + sum));
					echo $sum;
					
				fi
				
				i=$((i+1))
			
			fi
				
		done
			
			echo "St. studentov: $i";
			echo "Povprecne tocke: `expr $sum_global / $i`";
			
}

function drevo {
	imenik=${1:-.}
	globina=${2:-3}
	shopt -s nullglob
	
	function dr {
		local imenik=$1 tekovna=$2 globina=$3
		
		if [ "$tekovna" -gt "$globina" ]; then
			return
		fi
		
		if [ "$tekovna" -gt "0" ]; then
			printf -- '----%.0s' $(seq "$tekovna")
		fi
		
		local tip=$(tip "$imenik")
		
		printf "%-5s " $tip
		echo "$imenik"
		
		if [ "$tip" != "DIR" ]; then
			return
		fi
		
		pushd "$imenik" > /dev/null
		for f in *; do
			dr "$f" "$(($tekovna + 1))" "$globina"
		done
		popd > /dev/null
	}
	
	dr "$imenik" "0" "$globina"
	
}

function prostor {
	imenik=${1:-.}
	globina=${2:-3}
	shopt -s nullglob
	
	velikost=0;
	blokov=0;
	prostor=0;
	
	function pro {
		local imenik=$1 tekovna=$2 globina=$3
		
		if [ "$tekovna" -gt "$globina" ]; then
			return
		fi
		
		velikost=$(( velikost + $(stat -c %s "$imenik") ));
		blokov=$(( blokov + $(stat -c %b "$imenik") ));
		prostor=$(( prostor + $(stat -c %b "$imenik") * $(stat -c %B "$imenik") ));
		
		local tip=$(tip "$imenik")
		
		if [ "$tip" != "DIR" ]; then
			return
		fi
		
		pushd "$imenik" > /dev/null
		for f in *; do
			pro "$f" "$(($tekovna + 1))" "$globina"
		done
		popd > /dev/null
	}
	
	pro "$imenik" "0" "$globina"
	
	echo "Velikost: $velikost";
	echo "Blokov: $blokov";
	echo "Prostor: $prostor";
	
}
	
function tip {
	local file=$1
	if [ -L "$file" ]; then
		echo "LINK";
	elif [ -f "$file" ]; then
		echo "FILE";
	elif [ -d "$file" ]; then
		echo "DIR";
	elif [ -c "$file" ]; then
		echo "CHAR";
	elif [ -b "$file" ]; then
		echo "BLOCK";
	elif [ -p "$file" ]; then
		echo "PIPE";
	elif [ -s "$file" ]; then
		echo "SOCK";
	fi
}

case $1 in

	pomoc)
		pomoc;
		;;

	hehho)
		hehho "${@:2}";
		;;
	status)
		status ${@:2};
		;;
	leto)
		leto ${@:2};
		;;
	fib)
		fib ${@:2};
		;;
	upori)
		upori ${@:2};
		;;
	tocke)
		tocke;
		;;
	stej)
		stej ${@:2};
		;;
	drevo)
		drevo "${@:2}";
		;;
	prostor)
		prostor "${@:2}";
		;;
	*)
		echo "Napacna uporaba skripte!";
		echo "Uporaba: Naloga1.sh akcija parametri";
		exit 42;
		;;
	esac