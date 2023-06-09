# Inline Vector

### Задача

Реализуйте аналог `std::vector`, но у которого есть еще 1 шаблонный параметр - N. Т.е. до N элементов аллокации в куче не происходит, а если это число превышается, то уже начинается работа с динамической памятью.

### Порядок работы

1. Создаем директорию `build` и переходим в неё

	```bash
	mkdir build && cd build
	```

2. Подготавливаем необходимые конфигурационные файлы, а также `Makefile`

	```bash
	cmake ..
	```

3. Собираем

	```bash
	make
	```

4. Запускаем тесты

	```bash
	ctest
	```

### Описание работы

При иницилизации создается массив с данными для случая, когда размер вектора меньше указанного объема и создается объект для работы с динамической памятью. В методах: `operator=`, `capacity`, `operator[]`, `front`, `back`, `push_back`, `pop_back`, `begin`, `end`, `clear`, `insert`, `erase` - происходит проверка текущего размера с максимальным хранимым в массиве. В случае если это правда, то все указатели или операции проводятся над массивом, в ином случае выделяется память под недостающие элементы и далее работа происходит с кучей.

Для того чтоб поддержать схожий интерфейс работы, было решено создать отдельный класс для работы с кучей. В примере ниже видно, что если обновленный размер не превышает заданного размера, мы добавляем элемент в конец. Если же превышает, мы выделяем достаточный объем памяти и добавляем в кучу. Если происходит переход мы копируем элементы из массива в кучу. Аналогично и в обратном случае (копируем из кучи в массив).

```c++
template<typename T, std::size_t N, typename Allocator = std::allocator<T>>
void push_back(T& value) {
    if (size_ < N) {
        sized_data[size_++] = value;
    } else {
        dyn_data.reserve(size_ + 1);
        if (size_ == N)
            sized_to_dyn();
        dyn_data[size_++] = value;
    }
}
```

### Реализация

#### Codestyle

Был использован [кодстайл Стенфордского университета](http://stanford.edu/class/archive/cs/cs106b/cs106b.1158/styleguide.shtml) с небольшими отхождениями.

#### InlineVector

##### Конструкторы

Были созданы базовый конструктор, конструктор копирования и оператор присваивания, а также конструктор с массивом элементов как аргументом.

```c++
int main() {
    InlineVector<int, 4> vec1;
    InlineVector<int, 4> vec2 = {1, 2, 3};
    InlineVector<int, 4> vec3(vec2);
    // ...
    return 0;
}
```

Был создан деструктор, который очищает динамическую память при надобности.

##### Синонимы типов

Для упрощения работы с типами было решено использовать ключевое слово `using`. Например, `const_reference` является синонимом `const T&`, a `size_type` - `std::size_t`, что значительно упрощает написание кода и улучшает восприятие.

```c++
const_reference operator[](size_type index) const
```

Используемы типы:

```c++
using size_type = std::size_t;
using value_type = T;
using pointer = T*;
using const_pointer = const T*;
using reference = T&;
using const_reference = const T&;
using iterator = T*;
using const_iterator = const T*;
```

##### Методы

1. `size` - размер вектора
2. `capacity` - максимальный размер (N если кол-во элементов меньше или равно N и степени двойки в ином случае)
3. `empty` - пустой ли вектор
4. `operator[]` - оператор работы с элементами вектора
5. `front` - первый элемент
6. `back` - последний элемент
7. `push_back` - вставить элемент в конец
8. `pop_back` - удалить элемент с конца
9. `begin` - итератор указывающий на начало
10. `end` - итератор указывающий на конец
11. `clear` - очистить вектор
12. `insert` - вставить элемент
13. `erase` - удалить элемент
14. `operator==` - оператор сравнения InlineVector с InlineVector

##### Приватные методы

1. `sized_to_dyn` - метод для переноса данных фиксированно размера в динамическую память
2. `dyn_to_sized` - метод обратный методу `sized_to_dyn`

##### Атрибуты

1. `size_` - текущий размер вектора
2. `dyn_data` - объект для работы с данными (динамического размера)
3. `sized_data` - объект для работы с данными (фиксированного размера)

#### DynData

Данный класс является оберткой над данными выделенными динамически. Внутри него реализованы операторы и методы для поддержания общего интерфейса для работы с данными.

##### Конструкторы

Конструктор позволяет инициализировать аллокатор и установить значения по умолчанию для атрибутов. Деструктор очищает данные, если это необходимо.

##### Методы

1. `clear` - метод очистки динамической памяти
2. `operator[]` - оператор для получения и присваивания значений
3. `begin`, `end` - указатели на начало и конец
4. `size`, `capacity` - методы получения размеров
5. `reserve` - метод для обновления размера, в случае если текущего объема не достаточно выделяется новая память (максимальный объем памяти удваивается)

##### Атрибуты

1. `allocator_` - аллокатор
2. `data_` - динамический массив с данными
3. `capacity_` - текущий максимальный доступный объем
4. `size_` - текущий размер