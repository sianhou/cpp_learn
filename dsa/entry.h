//
// Created by sianh on 2023/6/23.
//

#ifndef LEARN_CPP_DSA_ENTRY_H_
#define LEARN_CPP_DSA_ENTRY_H_

template<typename K, typename V>
struct Entry {
    K key;
    V value;
    Entry(K k = K(), V v = V()) : key(k), value(v) {}
    Entry(const Entry<K, V> &e) : key(e.key), value(e.value) {}
    bool operator<(const Entry<K, V> &other) { return key < other.key; }
    bool operator>(const Entry<K, V> &other) { return key > other.key; }
    bool operator==(const Entry<K, V> &other) { return key == other.key; }
    bool operator!=(const Entry<K, V> &other) { return key != other.key; }
};

#endif //LEARN_CPP_DSA_ENTRY_H_
