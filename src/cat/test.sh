files=("test1.txt" "test2.txt")
flags=("-e" "-s" "-T" "-E" "-v" "-b" "-n" "-t")

# Перебираем файлы
for file in "${files[@]}"
do
    # Перебираем флаги
    for flag in "${flags[@]}"
    do
        # Запускаем программу с флагами и файлами и сохраняем вывод в файл
        ./s21_cat "$flag" "$file" > "output_$flag_$file.txt"
        # Запускаем команду cat с флагами и файлами и сохраняем вывод в файл
        cat "$flag" "$file" > "output2_$flag_$file.txt"

        # Проверяем наличие файлов
        if [[ ! -f "output_$flag_$file.txt" ]]; then
            echo "Файл output_$flag_$file.txt не существует."
            exit 1
        fi    
        if [[ ! -f "output2_$flag_$file.txt" ]]; then
            echo "Файл output2_$flag_$file.txt не существует."
            exit 1    
        fi

        # Сравниваем вывод программы с ожидаемым выводом
        diff "output_$flag_$file.txt" "output2_$flag_$file.txt"

        # Проверяем код возврата команды diff
        if [[ $? -eq 0 ]]; then
            echo "Файлы output_$flag_$file.txt и output2_$flag_$file.txt идентичны для флага $flag и файла $file."
        else
            echo "Файлы output_$flag_$file.txt и output2_$flag_$file.txt отличаются для флага $flag и файла $file."
        fi

        # Удаляем временный файл
        rm output_$flag_$file.txt output2_$flag_$file.txt
    done
done