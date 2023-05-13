#include "iostream"

namespace sj {

template<typename T>
class ListItem {
 public:

    ListItem() {
        _next = nullptr;
        _value = T();
    }

    ListItem(T value) {
        _value = value;
    }

    T value() const {
        return _value;
    }
    ListItem *next() const {
        return _next;
    }

    void set_next(ListItem<T> *ptr) {
        this->_next = ptr;
    }

 private:
    T _value;
    ListItem *_next;
};

template<typename T>
class List {
 public:

    List() {
        _tail = new ListItem<T>();
        _head = new ListItem<T>();
        _head->set_next(_tail);
        _size = 0;
    }

    void insert_front(T value) {
        ListItem<T> *tmp = new ListItem<T>(value);
        tmp->set_next(_head->next());
        _head->set_next(tmp);
        _size++;

    }
    void insert_end(T value) {
        auto item = _head;
        auto tmp = new ListItem<T>(value);
        while (item->next() != _tail) {
            item = item->next();
        }
        item->set_next(tmp);
        tmp->set_next(_tail);
    }

    void display(std::ostream &os = std::cout) const {
        for (auto item = _head->next(); item != _tail; item = item->next()) {
            os << item->value() << std::endl;
        }
    }

 private:
    ListItem<T> *_tail;
    ListItem<T> *_head;
    long _size;
};
}

int main() {
    sj::List<float> l;
    l.insert_end(1);
    l.insert_front(2);
    l.insert_front(3);
    l.insert_end(4);
    l.display();
}