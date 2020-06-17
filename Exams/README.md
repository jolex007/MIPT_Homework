# Практические задачи к экзамену по ООП 2019-2020 года

## !!! Задачи я не тестирую, так что если есть какие-то ошибки - пишите мне [@jolex007](https://t.me/jolex007) (Алексей Зерцалов)

Здесь будет список задачек, которые написаны в этом репозитории, и краткие поясняния к ним

- std::optional
- std::string
- biginteger
- std::array
- std::valarray
- std::advance & std::distance
- std::shared_ptr (директория выше)
- std::unique_ptr (директория выше)
- std::weak_ptr (директория выше)
- SFINAE method check
- SFINAE is_constructible

## Описания

### std::optional
Этот контейнер понадобится для ситуаций, когда вам нужно уметь хранить еще флаг, есть ли там элемент, или нет. Например, зачастую вы как "зарезервированное" число храните -1, INT_MAX, INT_MIN, и тд. Для этого и существует ```std::optional```, который реализован примерно как ```std::pair<T, bool>``` (bool - флаг, который показывает есть ли элемент, или нет)

Мне написали, что там нет работы с динамической памятью, поэтому его надо подправить, но, кажется, это запарнее будет, потому что придется возиться с байтами и тд

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

Thanks to [Tanya Micheeva](https://github.com/tanyaamiheeva)

### std::unqiue_ptr
Не особо понтовый вид умных указателей, поэтому их несложно писать)

### std::shared_ptr
Основная идея - храним динамическую структурку с нашим элементом, и счетчиком указателей. Пишется очень неприятно(

### std::weak_ptr
Надстройка над shared_ptr. В той структурке храним еще один счетчик. И указатель на эту структурку

### SFINAE method check
Проверяем есть ли в классе "Class" метод "name", для этого используем SFINAE

### SFINAE is_constructible
Примерно также, как и method check
