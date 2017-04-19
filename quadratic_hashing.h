// Bigram.h

#include "Bigram_base.h"
#include <vector>
#include <cmath>
#include <cstdlib>
#include "cmpt_error.h"
#include <iostream>

using namespace std;

//
// Put any other standard includes or helper functions/classes you need ...

// Entry structure containing a (key, value) pair to implement the hashtable. 
struct Entry {
    Bigram_key key;
    int val;
    int status = 0;
};

// Pre-condition:
// 	  None
// Post-condition:
//	  Returns a Polynomial hashCode for the given key with minimum collisions
unsigned long long hashCode(const Bigram_key& key) {
    string k = key.first + key.second;
    unsigned long long PolyhashCode = 0;
    int a = 37;
    for(int i = 0; i < k.size(); i++) {
        PolyhashCode += (unsigned long long)(pow(a, i)) * (unsigned long long)(k[i]);
    }
    return PolyhashCode;
}

// Pre-condition:
//	  None
// Post-condition:
// 	  Returns a Hash value that maps to an index of the BktArray
int Hash(unsigned long long hashCode, int SIZE) {
    return hashCode % SIZE;
}
//

class Bigram : public Bigram_base {

    // Implement all the non-implemented methods from Bigram_base here. Be
    // sure to include a default constructor that creates a new empty Bigram.
    
private:
    int n;
    vector<Entry> BktArray;
    Entry maxEnt;
    Entry minEnt;

public:
    // Pre-condition:
    //    None
    // Post-condition:
    //    Creates a new empty Bigram object when initialized: Bigram B;
    // Default Constructor
    Bigram() 
    : n(0), BktArray(100)
    { }
    
    // Destructor: Not needed to implement as the structure is made up of a std::vector
    ~Bigram() {
        
    }
    
    // Pre-condition:
    //    none
    // Post-condition:
    //    Returns the number of pairs in this map.
    // Performance:
    //    O(1) worst-case.
    int size() const {
        return n;
    }
    
    // Pre-condition:
    //    none
    // Post-condition:
    //    Returns the size of the underlying vector for this map.
    // Performance:
    //    O(1) worst-case.
    int capacity() const {
        return BktArray.size();
    }
    
    // Pre-condition:
    //    none
    // Post-condition:
    //    Returns true if there is a pair in this map with the given
    //    key, and false otherwise.
    // Performance:
    //    O(1) average-case number of comparisons.
    bool contains(const Bigram_key& key) const {
        unsigned long long hCode = hashCode(key);
        int hVal = Hash(hCode, capacity());
        if(BktArray[hVal].status != 0) {
            if(BktArray[hVal].key.first == key.first && BktArray[hVal].key.second == key.second) return true;
        }
        int i = 1;
        hVal = (hCode+1) % capacity();  
        while(BktArray[hVal].status != 0) {
            if(BktArray[hVal].key.first == key.first && BktArray[hVal].key.second == key.second) return true;
            i++;
            hVal = (hCode + int(pow(i, 2))) % capacity();
        }
        return false;
    }
    
    // Pre-condition:
    //    contains(key)
    // Post-condition:
    //    Returns the value associated with key.
    // Performance:
    //    O(1) average-case number of comparisons.
    int value_of(const Bigram_key& key) const {
        unsigned long long hCode = hashCode(key);
        int hVal = Hash(hCode, capacity());
        if(BktArray[hVal].status != 0) {    
            if(BktArray[hVal].key.first == key.first && BktArray[hVal].key.second == key.second) {
                return BktArray[hVal].val;
            }
        }
        int i = 1;
        hVal = (hCode+1) % capacity();  
        while(BktArray[hVal].status != 0) {
            if(BktArray[hVal].key.first == key.first && BktArray[hVal].key.second == key.second) {
                return BktArray[hVal].val;
            }
            i++;
            hVal = (hCode + int(pow(i, 2))) % capacity();
        }
        cmpt::error("The key is not in the vector");
        return -1;
    }
    
    // Pre-condition:
    //    none
    // Post-condition:
    //    contains(key) and value_of(key) == val
    // Performance:
    //    O(1) average-case number of comparisons.
    void put(const Bigram_key& key, int val) {
        if(load_factor() >= 0.5) {
            BktArray.resize(capacity() * 2);
        }
        unsigned long long hCode = hashCode(key);
        int hVal = Hash(hCode, capacity());
        
        // Initialize an Entry containing the new (key, value) pair to be inserted
        Entry ent;
        ent.key.first = key.first;
        ent.key.second = key.second;
        ent.val = val;
        ent.status = 1;
        
        // Check if it is the first key to be inserted
        // If so, initialize maxEnt and minEnt containing the entries with maximum and minimum associated values respectively.
        // Else, modify the previous maxEnt and minEnt if (val >= maxEnt.val) || (val <= minEnt.val)
        if(size() == 0) {
            maxEnt = ent;
            minEnt = ent;
        }
        else {
            if(val >= maxEnt.val) {
                if(val > maxEnt.val) {
                    maxEnt = ent;
                }
                else if(key <= maxEnt.key) {
                    maxEnt = ent;
                }
            }
            if(val <= minEnt.val) {
                if(val < minEnt.val) {
                    minEnt = ent;
                }
                else if(key <= minEnt.key) {
                    minEnt = ent;
                }
            }
        }
        
        if(BktArray[hVal].status != 0) {
            if(BktArray[hVal].key.first == key.first && BktArray[hVal].key.second == key.second) {
                BktArray[hVal].val = val;
                return;
            }
            int i = 1;
            int newhVal = (hCode + 1) % capacity();
            while(newhVal < BktArray.size()) {
                if(BktArray[newhVal].status == 0) {
                    BktArray[newhVal] = ent;
                    n++;
                    return;
                }
                else if(BktArray[newhVal].key.first == key.first && BktArray[newhVal].key.second == key.second) {
                    BktArray[hVal].val = val;
                    return;
                }
                i++;
                newhVal = (hCode + int(pow(i, 2))) % capacity();
            }
        }
        BktArray[hVal] = ent;
        n++;
        return;
    }

    // Pre-condition:
    //    none
    // Post-condition:
    //    !contains(key)
    // Performance:
    //    O(1) average-case number of comparisons.
    void remove(const Bigram_key& key) {
        unsigned long long hCode = hashCode(key);
        int hVal = Hash(hCode, capacity());
        if(BktArray[hVal].status != 0) {
            if(BktArray[hVal].key.first == key.first && BktArray[hVal].key.second == key.second) {
                
                // Check if the entry to be removed has (val == maxEnt.val) || (val == minEnt.val)
                // If so, modify the maxEnt.val or minEnt.val accordingly
                // Hence, maxEnt or minEnt stores the next maximum or next minimum respectively.
                if(BktArray[hVal].val == maxEnt.val) {
                    maxEnt = minEnt;
                    BktArray[hVal].status = 0;
                    for(int j = 0; j < BktArray.size(); j++) {
                        if(BktArray[j].status != 0 && BktArray[j].val >= maxEnt.val) {
                            if(BktArray[j].val > maxEnt.val) {
                                maxEnt = BktArray[j];
                            }
                            else if(BktArray[j].key <= maxEnt.key) {
                                maxEnt = BktArray[j];
                            }
                        }
                    }
                }
                if(BktArray[hVal].val == minEnt.val) {
                    minEnt = maxEnt;
                    BktArray[hVal].status = 0;
                    for(int j = 0; j < BktArray.size(); j++) {
                        if(BktArray[j].status != 0 && BktArray[j].val <= minEnt.val) {
                            if(BktArray[j].val < minEnt.val) {
                                minEnt = BktArray[j];
                            }
                            else if(BktArray[j].key <= minEnt.key) {
                                minEnt = BktArray[j];
                            }
                        }
                    }
                }
                
                BktArray[hVal].status = 0;
                n--;
                return;
            }
        }
        int i = 1;
        hVal = (hCode + 1) % capacity();
        while(BktArray[hVal].status != 0) {
            if(BktArray[hVal].key.first == key.first && BktArray[hVal].key.second == key.second) {
                BktArray[hVal].status = 0;
                n--;
                return;
            }
            i++;
            hVal = (hCode + int(pow(i, 2))) % capacity();
        }
        return;
    }

    // Pre-condition:
    //    !is_empty()
    // Post-condition:
    //    Returns the key in this map with the largest associated
    //    value. If there is more than one (key, value) pair tied
    //    for the largest value, then return the one with the smaller
    //    key (use operator<= defined above to compare them).
    // Performance:
    //    O(1) worst-case (not average-case!) number of comparisons.
    Bigram_key max() const {
        return maxEnt.key;
    }

    // Pre-condition:
    //    !is_empty()
    // Post-condition:
    //    Returns the key in this map with the smallest associated
    //    value. If there is more than one (key, value) pair tied
    //    for the smallest value, then return the one with the smaller
    //    key (use operator<= defined above to compare them).
    // Performance:
    //    O(1) worst-case (not average-case!) number of comparisons.
    Bigram_key min() const {
        return minEnt.key;
    }
    
    // Pre-condition:
    //    None
    // Post-condition:
    //    Prints all the (key, value) pairs in the 2-dimensional vector with their respective positions.
    // Performance:
    //    O(n) number of comparisons.
    void printBktArray() const {
        for(int i = 0; i < BktArray.size(); i++) {
                cout << "Index = " << i << " " << BktArray[i].key.first << " " << BktArray[i].key.second << " " << BktArray[i].val << "\n";
        }
    }

}; // class Bigram