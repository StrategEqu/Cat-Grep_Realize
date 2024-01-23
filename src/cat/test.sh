#!/bin/bash
SUCCESS='\e[32m'
FAILURE='\033[0;31m'
MENU='\e[33m'
RESET='\033[0m'

file="test1.txt"
compile="s21_cat"
flags=("-e" "-s" "-T" "-E" "-b" "-n" "-t" "-v" "--number"
	"--number-nonblank" "--squeeze-blank" "--show-ends" "--show-tabs")
success_count=0
failure_count=0

printf "${MENU}%s==============================================Compatibility_Test==============================================%s${RESET}\n"
# Перебираем одиночные флаги
for flag in "${flags[@]}"; do
	# Проверяем наличие скомпилированного файла
	if [[ ! -f "$compile" ]]; then
		echo "$compile file does not exist."
		exit 1
	fi
	# Проверяем наличие файлов и удаляем временные файлы, если false
	if [[ ! -f "$file" ]]; then
		echo "$file file does not exist."
		rm "output_${flag}_${file}.txt" "output2_${flag}_${file}.txt"
		exit 1
	fi

	# Запускаем программу с флагами и файлами и сохраняем вывод в файл
	"./$compile" "$flag" "$file" >"output_${flag}_${file}.txt"
	# Запускаем команду cat с флагами и файлами и сохраняем вывод в файл
	cat "$flag" "$file" >"output2_${flag}_${file}.txt"

	# Сравниваем вывод программы с ожидаемым выводом
	if diff "output_${flag}_${file}.txt" "output2_${flag}_${file}.txt"; then
		# echo -e "${SUCCESS}[SUCCESS]${RESET} The commands are identical for the $flag flag, the $file file."
		((success_count++))
	else
		# echo -e "${FAILURE}[FAIL]${RESET} The commands are different for the $flag flag, the $file file."
		((failure_count++))
	fi

	# Удаляем временные файлы
	rm "output_${flag}_${file}.txt" "output2_${flag}_${file}.txt"
done

# Перебираем парные флаги
for ((i = 0; i < ${#flags[@]}; i++)); do
	flag1=${flags[i]}

	for ((j = i + 1; j < ${#flags[@]}; j++)); do
		flag2=${flags[j]}

		"./$compile" "$flag1" "$flag2" "$file" >"output_${flag1}_${flag2}_${file}.txt"
		cat "$flag1" "$flag2" "$file" >"output2_${flag1}_${flag2}_${file}.txt"

		if diff "output_${flag1}_${flag2}_${file}.txt" "output2_${flag1}_${flag2}_${file}.txt"; then
			# echo -e "${SUCCESS}[SUCCESS]${RESET} The commands are identical for the $flag1 and $flag2 flags, the $file file"
			((success_count++))
		else
			# echo -e "${FAILURE}[FAIL]${RESET} The commands differ for the $flag1 and $flag2 flags, the $file file"
			((failure_count++))
		fi
		rm "output_${flag1}_${flag2}_${file}.txt" "output2_${flag1}_${flag2}_${file}.txt"
	done
done

# printf "${MENU}%s--------------------------------------------------------------------------------------------------------------%s${RESET}\n"
if [[ $failure_count == 0 ]]; then
	echo -e "${SUCCESS}[SUCCESS]${RESET} Total successful comparisons: $success_count/$failure_count"
else
	echo -e "${FAILURE}[FAIL]${RESET} Total successful comparisons: $success_count/$failure_count"
fi
printf "${MENU}%s================================================Test_completed================================================%s${RESET}\n"
