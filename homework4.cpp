/** 
 * Author: Blake Boswell
 * 
 * For each of linear probing, quadratic probing, and double hashing do the following:
 * Hash 900 random numbers into a table of length 1009 using open addressing
 * then add 50 more random integers currently not in the table to the table
 * while counting the total number of probes for the last 50.
 * 
 * Use the hash functions given in exercise 11.4-1.
 * auxiliary hash function: h(k) = k
 * h_1(k) = k
 * h_2(k) = 1 + (k mod (m - 1))
 * c_1 = 1
 * c_2 = 3
*/
#include <iostream>
#include <stdexcept>
#include <stdlib.h>
#include <time.h>

using namespace std;

const int TABLE_SIZE = 1009;
const int MAX_RAND = RAND_MAX - 50;

class HashTable {
    private:
        int* table;
        int size;
        int usedSlots;
        char probeType;
        const static int c1 = 1;
        const static int c2 = 3;

        /**
         * Linear probe insert
         * @return value: probeCount
         */
        int insertLP(int k) {
            int probeCount = 0;
            while (probeCount != this->size) {
                int j = linearProbe(k, probeCount);
                if (this->table[j] == -1) {
                    this->table[j] = k;
                    this->usedSlots++;
                    return probeCount;
                } else {
                    probeCount++;
                }
            }
            throw range_error("hash table overflow");
        }

        /**
         * Linear probe search
         */
        int searchLP(int k) {
            for (int i = 0; i < this->size; i++) {
                int j = linearProbe(k, i);
                if (this->table[j] == -1) {
                    return -1;
                }
                if (this->table[j] == k) {
                    return j;
                }
            }
            return -1;
        }

        /**
         * Quadratic probe insert
         * @return value: probeCount
         */
        int insertQP(int k) {
            int probeCount = 0;
            while (probeCount != this->size) {
                int j = quadraticProbe(k, probeCount);
                // cout << "Attempting to insert into slot " << j << endl;
                if (this->table[j] == -1) {
                    // cout << "Slot j equals " << this->table[j] << endl;
                    this->table[j] = k;
                    // cout << "Slot " << j << " now equals " << k << endl;
                    this->usedSlots++;
                    return probeCount;
                } else {
                    probeCount++;
                }
            }
            throw range_error("hash table overflow");
        }

        /**
         * Quadratic probe search
         */
        int searchQP(int k) {
            for (int i = 0; i < this->size; i++) {
                int j = quadraticProbe(k, i);
                if (this->table[j] == -1) {
                    return -1;
                }
                if (this->table[j] == k) {
                    return j;
                }
            }
            return -1;
        }

        /**
         * Double hash insert
         * @return value: probeCount
         */
        int insertDH(int k) {
            int probeCount = 0;
            while (probeCount != this->size) {
                int j = doubleHash(k, probeCount);
                // cout << "Attempting to insert into slot " << j << endl;
                if (this->table[j] == -1) {
                    // cout << "Slot j equals " << this->table[j] << endl;
                    this->table[j] = k;
                    // cout << "Slot " << j << " now equals " << k << endl;
                    this->usedSlots++;
                    return probeCount;
                } else {
                    probeCount++;
                }
            }
            throw range_error("hash table overflow");
        }

        /**
         * Double hash search
         */
        int searchDH(int k) {
            for (int i = 0; i < this->size; i++) {
                int j = doubleHash(k, i);
                if (this->table[j] == -1) {
                    return -1;
                }
                if (this->table[j] == k) {
                    return j;
                }
            }
            return -1;
        }
        
    public:
        const static unsigned char DOUBLE_HASH = 'D';
        const static unsigned char LINEAR_PROBE = 'L';
        const static unsigned char QUADRATIC_PROBE = 'Q';

        HashTable(int size, char probeType = 'D') {
            this->size = size;
            if (probeType == 'D' || probeType == 'L' || probeType == 'Q') {
                this->probeType = probeType; 
            } else {
                this->probeType = 'D';
            }
            this->probeType = probeType;
            this->table = new int[size];
            this->usedSlots = 0;
            for (int i = 0; i < size; i++) {
                this->table[i] = -1;
            }
        }

        ~HashTable() {
            delete [] table;
        }

        int hash(int k) {
            return k;
        }

        int hash1(int k) {
            return k;
        }

        int hash2(int k) {
            return (1 + (k % (this->size - 1)));
        }

        int linearProbe(int k, int i) {
            return ((hash(k) + i) % this->size);
        }

        int quadraticProbe(int k, int i) {
            return ((hash(k) + (c1 * i) + (c2 * i * i)) % this->size);
        }

        int doubleHash(int k, int i) {
            return ((hash1(k) + (i * hash2(k))) % this->size);
        }

        int insert(int k) {
            if (this->probeType == 'L') {
                return insertLP(k);
            } else if (this->probeType == 'Q') {
                return insertQP(k);
            } else if (this->probeType == 'D') {
                return insertDH(k);
            }
            return -1;
        }

        int search(int k) {
            if (this->probeType == 'L') {
                return searchLP(k);
            } else if (this->probeType == 'Q') {
                return searchQP(k);
            } else if (this->probeType == 'D') {
                return searchDH(k);
            }
            return -1;
        }

        int operator [](int key) {
            if (key < this->size && key >= 0) {
               return this->table[key]; 
            }
            throw out_of_range("Out of range");
        }

        int getUsedSlots() {
            return this->usedSlots;
        }


};

int main(int argc, char** argv) {
    srand(time(NULL));
    int probeCount;

    // --------------------- QUADRATIC PROBING -----------------------

    cout << "Quadratic Probing\n\n";
    HashTable tableQP (TABLE_SIZE, HashTable::QUADRATIC_PROBE);
    
    // Adding 900 random integers 50 - MAX_RAND
    for (int i = 0; i < 900; i++) {
        int random = rand() % MAX_RAND + 50;
        tableQP.insert(random);
    }

    // Adding 50 new integers to count probes
    probeCount = 0;

    int randomKeys[50];
    for (int i = 0; i < 50; i++) {
        randomKeys[i] = i;
    }
    // Shuffle
    for (int i = 0; i < 50; i++) {
        int random = rand() % 50;
        int temp = randomKeys[i];
        randomKeys[i] = randomKeys[random];
        randomKeys[random] = temp;
    }

    for (int i = 0; i < 50; i++) {
        // int random = rand() % 50;
        int numProbes = tableQP.insert(randomKeys[i]);
        probeCount += numProbes;
    }

    cout << "\t" << tableQP.getUsedSlots() << endl;
    cout << "\tProbe Count for last 50: " << probeCount << endl;
    cout << "\tAverage probes for last 50: " << (float)probeCount/50 << endl;

    // --------------------- LINEAR PROBING -----------------------

    cout << "Linear Probing\n\n";
    HashTable tableLP (TABLE_SIZE, HashTable::LINEAR_PROBE);

    // Adding 900 random integers 50 - MAX_RAND
    for (int i = 0; i < 900; i++) {
        int random = rand() % MAX_RAND + 50;
        tableLP.insert(random);
    }

    // Adding 50 new integers to count probes
    probeCount = 0;

    // Shuffle
    for (int i = 0; i < 50; i++) {
        int random = rand() % 50;
        int temp = randomKeys[i];
        randomKeys[i] = randomKeys[random];
        randomKeys[random] = temp;
    }

    for (int i = 0; i < 50; i++) {
        // int random = rand() % 50;
        int numProbes = tableLP.insert(randomKeys[i]);
        probeCount += numProbes;
    }

    cout << "\t" << tableLP.getUsedSlots() << endl;
    cout << "\tProbe Count for last 50: " << probeCount << endl;
    cout << "\tAverage probes for last 50: " << (float)probeCount/50 << endl;

    // --------------------- DOUBLE HASHING -----------------------

    cout << "Double Hashing\n\n";
    HashTable tableDH (TABLE_SIZE, HashTable::DOUBLE_HASH);

    // Adding 900 random integers 50 - MAX_RAND
    for (int i = 0; i < 900; i++) {
        int random = rand() % MAX_RAND + 50;
        tableDH.insert(random);
    }

    // Adding 50 new integers to count probes
    probeCount = 0;

    // Shuffle
    for (int i = 0; i < 50; i++) {
        int random = rand() % 50;
        int temp = randomKeys[i];
        randomKeys[i] = randomKeys[random];
        randomKeys[random] = temp;
    }

    for (int i = 0; i < 50; i++) {
        // int random = rand() % 50;
        int numProbes = tableDH.insert(randomKeys[i]);
        probeCount += numProbes;
    }

    cout << "\t" << tableDH.getUsedSlots() << endl;
    cout << "\tProbe Count for last 50: " << probeCount << endl;
    cout << "\tAverage probes for last 50: " << (float)probeCount/50 << endl;
    return 0;
}