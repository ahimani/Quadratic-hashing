// Bigram_base.h

#include <string>

using namespace std;

struct Bigram_key {
    string first;
    string second;
};

//
// Defines a comparison operator for Bigram_key objects, letting you write code
// like this:
//
//   // s and t are previously defined Bigram_key objects
//   if (s <= t) {
//     // ... s is equal to t, or comes before it
//   } else {
//     // ... s comes after t ...
//   }
//
bool operator<=(const Bigram_key& a, const Bigram_key& b) {
    if (a.first == b.first) {
        return a.second <= b.second;;
    } else {
        return a.first <= b.first;
    }
}

//
// Bigram_base is an abstract base class for a map that stores (key, value)
// pairs, where the key is a Bigram and its associated value is an int.
//
class Bigram_base {
public:
    // Abstract base classes should always include a virtual destructor so
    // that inheriting classes can, if they need to, implement their own
    // destructor.
    virtual ~Bigram_base() { }

    // Pre-condition:
    //    none
    // Post-condition:
    //    Returns the number of pairs in this map.
    // Performance:
    //    O(1) worst-case.
    virtual int size() const = 0;

    // Pre-condition:
    //    none
    // Post-condition:
    //    Returns the size of the underlying vector for this map.
    // Performance:
    //    O(1) worst-case.
    virtual int capacity() const = 0;

    // Pre-condition:
    //    none
    // Post-condition:
    //    Returns the load factor for this map.
    //    If the capacity is 0, then 0 is returned.
    // Performance:
    //    O(1) worst-case.
    double load_factor() const {
        if (capacity() == 0) {
            return 0;
        } else {
            return double(size()) / capacity();
        }
    }

    // Pre-condition:
    //    none
    // Post-condition:
    //    Returns true if there is a pair in this map with the given
    //    key, and false otherwise.
    // Performance:
    //    O(1) average-case number of comparisons.
    virtual bool contains(const Bigram_key& key) const = 0;

    // Pre-condition:
    //    contains(key)
    // Post-condition:
    //    Returns the value associated with key.
    // Performance:
    //    O(1) average-case number of comparisons.
    virtual int value_of(const Bigram_key& key) const = 0;

    // Pre-condition:
    //    none
    // Post-condition:
    //    contains(key) and value_of(key) == val
    // Performance:
    //    O(1) average-case number of comparisons.
    virtual void put(const Bigram_key& key, int val) = 0;

    // Pre-condition:
    //    none
    // Post-condition:
    //    !contains(key)
    // Performance:
    //    O(1) average-case number of comparisons.
    virtual void remove(const Bigram_key& key) = 0;

    // Pre-condition:
    //    !is_empty()
    // Post-condition:
    //    Returns the key in this map with the largest associated
    //    value. If there is more than one (key, value) pair tied
    //    for the largest value, then return the one with the smaller
    //    key (use operator<= defined above to compare them).
    // Performance:
    //    O(1) worst-case (not average-case!) number of comparisons.
    virtual Bigram_key max() const = 0;

    // Pre-condition:
    //    !is_empty()
    // Post-condition:
    //    Returns the key in this map with the smallest associated
    //    value. If there is more than one (key, value) pair tied
    //    for the smallest value, then return the one with the smaller
    //    key (use operator<= defined above to compare them).
    // Performance:
    //    O(1) worst-case (not average-case!) number of comparisons.
    virtual Bigram_key min() const = 0;
}; // class Bigram_base
