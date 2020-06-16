# Практические задачи к экзамену по ООП 2019-2020 года

## !!! Задачи я не тестирую, так что если есть какие-то ошибки - пишите мне [@jolex007](https://t.me/jolex007) (Алексей Зерцалов)

Здесь будет список задачек, которые написаны в этом репозитории, и краткие поясняния к ним

- std::optional
- std::string

## Описания

### std::optional
Этот контейнер понадобится для ситуаций, когда вам нужно уметь хранить еще флаг, есть ли там элемент, или нет. Например, зачастую вы как "зарезервированное" число храните -1, INT_MAX, INT_MIN, и тд. Для этого и существует std::optional, который реализован примерно как std::pair<T, bool> (bool - флаг, который показывает есть ли элемент, или нет)

### std::string
Ну, если захотите сюда вписать свою рекламу - пишите) Надеюсь там все понятно)