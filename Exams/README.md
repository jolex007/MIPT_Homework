# Практические задачи к экзамену по ООП 2019-2020 года

## !!! Задачи я не тестирую, так что если есть какие-то ошибки - пишите мне [@jolex007](https://t.me/jolex007) (Алексей Зерцалов)

Здесь будет список задачек, которые написаны в этом репозитории, и краткие поясняния к ним

- std::optional
- std::string
- biginteger
- std::array
- std::valarray
- std::advance & std::distance

## Описания

### std::optional
Этот контейнер понадобится для ситуаций, когда вам нужно уметь хранить еще флаг, есть ли там элемент, или нет. Например, зачастую вы как "зарезервированное" число храните -1, INT_MAX, INT_MIN, и тд. Для этого и существует std::optional, который реализован примерно как std::pair<T, bool> (bool - флаг, который показывает есть ли элемент, или нет)

### std::string
Ну, если захотите сюда вписать свою рекламу - пишите) Надеюсь там все понятно)

Thanks to [Tanya Micheeva](https://github.com/tanyaamiheeva)

### biginteger
Тут снова вроде все понятно, аккуратно пишем все)

Thanks to [Tanya Micheeva](https://github.com/tanyaamiheeva)

### std::array
Просто надстройка над std::array

Thanks to [Tanya Micheeva](https://github.com/tanyaamiheeva)

### std::valarray
Очень удобные массивы, почти как в питоне
Пример:
```cpp
std::valarray<int> a = {1, 2, 3};
std::valarray<int> b = {1, 2, 3};
std::valarray<int> c = a + b;
/*
c = {2, 4, 6}
*/
```
Ну и также очень много всего полезного)

### std::advance & std::distance
std::advance сдвигает итератор на n шагов
std::distance считает расстояние между итераторами

Проблема их в том, что они могут быть как Random access, так и не Random access