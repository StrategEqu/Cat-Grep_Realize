#!/bin/bash
SUCCESS='\e[32m'
RESET='\033[0m'
FAILURE='\033[0;31m'

file="test1.txt"
pattern="pattern.txt"
compile="s21_grep"
flags=("-v" "-i" "-o" "-l" "-n" "-c" "-e" "-f" "-s" "-h")

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
	./s21_grep "$flag" "$pattern" "$file" >"output_${flag}_${file}.txt"
	# Запускаем команду grep с флагами и файлами и сохраняем вывод в файл
	grep "$flag" "$pattern" "$file" >"output2_${flag}_${file}.txt"

	# Сравниваем вывод программы с ожидаемым выводом
	if diff "output_${flag}_${file}.txt" "output2_${flag}_${file}.txt"; then
		echo -e "${SUCCESS}[SUCCESS]${RESET} The commands are identical for the $flag flag, the $file file, and the $pattern pattern."
	else
		echo -e "${FAILURE}[FAIL]${RESET} The commands are different for the $flag flag, the $file file, and the $pattern pattern."
	fi

	# Удаляем временные файлы
	rm "output_${flag}_${file}.txt" "output2_${flag}_${file}.txt"
done

# Перебираем парные флаги
for ((i = 0; i < ${#flags[@]}; i++)); do
	flag1=${flags[i]}

	for ((j = i + 1; j < ${#flags[@]}; j++)); do
		flag2=${flags[j]}

		./s21_grep "$flag1" "$pattern" "$file" >"output_${flag1}_${flag2}_${file}.txt"
		grep "$flag1" "$pattern" "$file" >"output2_${flag1}_${flag2}_${file}.txt"

		if diff "output_${flag1}_${flag2}_${file}.txt" "output2_${flag1}_${flag2}_${file}.txt"; then
			echo -e "${SUCCESS}[SUCCESS]${RESET} The commands are identical for the $flag1 and $flag2 flags, the $file file and the $pattern pattern"
		else
			echo -e "${FAILURE}[FAIL]${RESET} The commands differ for the $flag1 and $flag2 flags, the $file file and the $pattern pattern"
		fi

		rm "output_${flag1}_${flag2}_${file}.txt" "output2_${flag1}_${flag2}_${file}.txt"
	done
done
