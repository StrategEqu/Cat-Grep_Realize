SUCCESS='\e[32m'
MENU='\e[33m'
RESET='\033[0m'
FAILURE='\033[0;31m'

file="test2.txt"
compile="s21_grep"
flags=("-e" "-s" "-T" "-E" "-b" "-n" "-t" "--number" 
"--number-nonblank" "--squeeze-blank" "--show-ends" "--show-tabs")

    # Перебираем флаги
    for flag in "${flags[@]}"
    do
            # Проверяем наличие скомпилированного файла
        if [[ ! -f "$compile" ]]; then 
            echo "Файл $compile не существует."
            exit 1
        fi
            # Проверяем наличие файлов и удаляем временные файлы, если false
        if [[ ! -f "$file" ]]; then
            echo "Файл $file не существует."
            rm output_$flag_$file.txt output2_$flag_$file.txt
            exit 1
        fi    

        # Запускаем программу с флагами и файлами и сохраняем вывод в файл
        ./s21_cat "$flag" "$file" > "output_$flag_$file.txt"
        # Запускаем команду cat с флагами и файлами и сохраняем вывод в файл
        cat "$flag" "$file" > "output2_$flag_$file.txt"

        # Сравниваем вывод программы с ожидаемым выводом
        diff "output_$flag_$file.txt" "output2_$flag_$file.txt"

        # Проверяем код возврата команды diff
        if [[ $? -eq 0 ]]; then
            echo -e "${SUCCESS}[SUCCESS]${RESET} Команды идентичны для флага $flag и файла $file."
        else
            echo -e "${FAILURE}[FAIL]${RESET} Команды отличаются для флага $flag и файла $file."
        fi

        # Удаляем временный файл
        rm output_$flag_$file.txt output2_$flag_$file.txt
    done