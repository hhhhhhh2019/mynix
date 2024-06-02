На остальные файлы(кроме LANG.md) пока не смотрите - они не актуальны.

# Как это работает?

Файл конфигурации возвращает 2 значения: репозитории и сама конфигурация системы.

Пакет - это git репозиторий в котором должны быть:
    * package.dbpm - основной файл пакета, содержит: зависимости, имя, описание
    * preBuild.sh (опционально)
    * build.sh
    * postBuild.sh (опционально)
    * preInstall.sh (опционально)
    * install.sh
    * postInstall.sh (опционально)
    * src (может являтся подмодулем)

Всем .sh файлам в корне пакета передаётся конфигурация системы(или пользователя)
