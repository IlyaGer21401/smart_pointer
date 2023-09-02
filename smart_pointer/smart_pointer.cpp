#include <iostream>
#include <string>
#include <ctime>
using namespace std;

#define PROVERKA 0

// Тестовый класс, на котором тестируем указатель
class Toy {
private:
    int ind_number; // Номер игрушки
    string name;    // Имя игрушки
public:
    // Конструкторы класса
    Toy(string _name, int _ind_number) : name(_name) {
        if (ind_number > 1)
            ind_number = (rand() % 200) + 1;
        else
            ind_number = _ind_number;
    }
    Toy() : name("SomeToy"), ind_number(ind_number = (rand() % 200) + 1) {}
    Toy(string _name) : Toy(_name, ind_number = (rand() % 200) + 1) {}
    Toy(int _ind_number) : Toy("SomeToy", _ind_number) {}

    // Конструктор копирования
    Toy(const Toy& other) : ind_number(other.ind_number), name(other.name) {}
    // Конструктор присваивания
    Toy& operator=(const Toy& other) {
        if (this == &other)
            return *this;
        name = other.name;
        ind_number = other.ind_number;
        return *this;
    }

    // Вывод информации о классе
    void getInfo() { 
        cout << "=====================================\n";
        cout << "Identification number: " << ind_number << endl;
        cout << "Name toy:" << name << endl;
    }
};

/// Мой умный указатель 
class shared_ptr_toy {
private:
    Toy* obj; // На что указываем
    int* count; // Количество ссылок
public:
    // Конструкторы класса
    shared_ptr_toy() {
        obj = new Toy();
        count = new int(1);
    }
    shared_ptr_toy(string name) {
        obj = new Toy(name);
        count = new int(1);
    }
    shared_ptr_toy(int ind_number) {
        obj = new Toy(ind_number);
        count = new int(1);
    }
    shared_ptr_toy(string name, int ind_number) {
        obj = new Toy(name, ind_number);
        count = new int(1);
    }

    // Конструктор копирования
    shared_ptr_toy(const shared_ptr_toy& other) {
        obj = other.obj;
        count = other.count;
        ++*count;
    }
    
    // Конструктор присваивания
    shared_ptr_toy& operator=(const shared_ptr_toy& other) {
        if (this == &other)
            return *this;
        if (obj != nullptr) {
            *--count;
            if (*count == 0) {
                delete obj;
                delete count;
            }
            else {
                obj = nullptr;
            }
        }
        obj = other.obj;
        count = other.count;
        ++*count;
    }
    
    // Деструктор
    ~shared_ptr_toy() {
        --*count;
        if (*count == 0) {
            delete obj;
            delete count;
        }
    }
    
    
    Toy getObj() {
        return *obj;
    }
    void setObj(Toy* other) {
        obj = other;
    }
    int getCount() {
        return *count;
    }
};

Toy* make_shared_toy(string name) {
    Toy* toy = new Toy(name);
    return toy;
}
Toy* make_shared_toy(int ind_number) {
    Toy* toy = new Toy(ind_number);
    return toy;
}
Toy* make_shared_toy(string name, int ind_number) {
    Toy* toy = new Toy(name, ind_number);
    return toy;
}
Toy* make_shared_toy(const Toy& oth_toy) {
    Toy* toy = new Toy(oth_toy);
    return toy;
}

int main() {
    cout << "=====================================\n";
    cout << "\n\tTEST #1\n\n";
    cout << "=====================================\n";
    srand(time(nullptr));
#if PROVERKA == 0
    ///////////////////////////////////////////////////////////////////////////////////////////
//                                        ТЕСТ 1 
//                             Проверка на подсчет ссылок
    ///////////////////////////////////////////////////////////////////////////////////////////
    
    shared_ptr_toy ptr;
    ptr.setObj(make_shared_toy("Ball")); // Дали имя игрушке
    cout << "ptr::Count:\t" << ptr.getCount() << endl;
 
 // После создания 1го указателя кол-во ссылок равно 1 (count = 1)


    cout << "\n\n";
    shared_ptr_toy ptr2(ptr); // 
    cout << "ptr::Count:\t" << ptr.getCount() << endl;
    cout << "ptr2::Count:\t" << ptr2.getCount() << endl;

    // После создания 2го указателя кол-во ссылок на двух указателях равно 2 (count = 2)


    cout << "\n\n";
    shared_ptr_toy ptr3;
    ptr3 = ptr2;
    cout << "ptr::Count:\t" << ptr.getCount() << endl;
    cout << "ptr2::Count:\t" << ptr2.getCount() << endl;
    cout << "ptr3::Count:\t" << ptr3.getCount() << endl;
    cout << "=====================================\n";

    // После создания 3го указателя кол-во ссылок на двух указателях равно 3 (count = 3)


#else ///SHARED_PTR, Результат встроенного умного указателя

    shared_ptr<Toy> ptr = make_shared<Toy>("Ball");
    cout << ptr.use_count() << endl;
    shared_ptr<Toy> ptr2(ptr);
    cout << ptr2.use_count() << endl;
    shared_ptr<Toy> ptr3;
    ptr3 = ptr2;
    cout << ptr.use_count() << endl;
#endif

    cout << "\n\tTEST #2";

    ///////////////////////////////////////////////////////////////////////////////////////////
//                                        ТЕСТ 2 
//                     Проверка на то, что указатель работает исправно
    ///////////////////////////////////////////////////////////////////////////////////////////

    //Создаем 3 указателя называем их и даем номер
    shared_ptr_toy a("Bear", 1);
    shared_ptr_toy b("Ball", 2);
    shared_ptr_toy c;
    c.setObj(make_shared_toy("Ball", 3));

    cout << "\n\nTry 1\n";
    a.getObj().getInfo();
    b.getObj().getInfo();
    c.getObj().getInfo();

    a = b;

    // Приравниваем 'а' к 'b'
    cout << "\n\nTry 2\n";
    a.getObj().getInfo();
    b.getObj().getInfo();
    c.getObj().getInfo();

    a.setObj(make_shared_toy("Bear"));

    // Переименовываем 'а' в "Bear"
    cout << "\n\nTry 3\n";
    a.getObj().getInfo();
    b.getObj().getInfo();
    c.getObj().getInfo();

    c.setObj(make_shared_toy(a.getObj()));

    // Говорим указателю 'c', что он теперь указывает на 'a'
    cout << "\n\nTry 4\n";
    a.getObj().getInfo();
    b.getObj().getInfo();
    c.getObj().getInfo();

    return 0;

}