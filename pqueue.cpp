#include <stdlib.h>

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

#ifndef PQUEUE
#define PQUEUE

/**
 * General description:  priority queue class which stores pairs
 *   <id, priority>.  Top of queue is determined by priority
 *   (min or max depending on configuration).
 *
 *   There can be only one (or zero) entry for a particular id.
 *
 *   Capacity is fixed on creation.
 *
 *   IDs are integers in the range [0..N-1] where N is the capacity
 *   of the priority queue set on creation.  Any values outside this
 *   range are not valid IDs.
 **/
struct Node {
    double priority;
    int id;
};

class pqueue {
   private:  // maybe put your data members (and types if desired) here
    bool min;
    int cap;
    int firstEmpty = 1;
    vector<Node *> heap;
    vector<int> ids;

   public:
    /**
     * CONSTRUCTOR
     * Parameters: capacity - self-explanatory
     *             min_heap - if true (really non-zero), then it is a min-heap
     *                        if false, then a max-heap
     */

    pqueue(int capacity, bool min_heap = true) {
        min = min_heap;
        cap = capacity;
        heap.resize(cap + 1, nullptr);
        ids.resize(cap, 0);
    }

    /**
     * Destructor
     * Desc: deallocates all memory associated with passed priority
     *       queue.
     * Runtime: Theta(Capacity of Queue)
     */
    ~pqueue() {
        for (int i = 0; i < cap + 1; i++) {
            if (heap.at(i) != nullptr) {
                free(heap.at(i));
            }
        }
    }

    /**
     * Function: insert
     * Parameters: id of entry to insert
     *             priority of entry to insert
     * Returns: true on success; false on failure.
     *          fails if id is out of range or
     *            there is already an entry for id
     *          succeeds otherwise.
     *
     * Runtime:  O(log n)
     *
     */
    bool insert(int id, double priority) {
        if (id >= cap || id < 0 || ids.at(id) != 0) {
            return false;
        }

        Node *temp = new Node();

        temp->id = id;
        temp->priority = priority;
        heap.at(firstEmpty) = temp;
        ids.at(id) = firstEmpty;

        if (firstEmpty != 1 && min && priority < heap.at(firstEmpty / 2)->priority) {
            perc_up_min(firstEmpty, id);
        } else if (firstEmpty != 1 && !min && priority > heap.at(firstEmpty / 2)->priority) {
            perc_up_max(firstEmpty, id);
        }
        firstEmpty++;
        return true;
    }

    void perc_up_min(int spot, int id) {
        while ((spot / 2) > 0 && heap.at(spot)->priority < heap.at(spot / 2)->priority) {
            Node *temp = heap.at(spot / 2);

            int oldID = heap.at(spot / 2)->id;
            ids.at(oldID) = spot;

            heap.at(spot / 2) = heap.at(spot);
            heap.at(spot) = temp;
            spot /= 2;
        }
        ids.at(id) = spot;
    }

    void perc_up_max(int spot, int id) {
        while ((spot / 2) > 0 && heap.at(spot)->priority > heap.at(spot / 2)->priority) {
            Node *temp = heap.at(spot / 2);

            int oldID = heap.at(spot / 2)->id;
            ids.at(oldID) = spot;

            heap.at(spot / 2) = heap.at(spot);
            heap.at(spot) = temp;
            spot /= 2;
        }
        ids.at(id) = spot;
    }

    /**
     * Function: change_priority
     * Parameters: element id
     *             new_priority
     * Returns: true on success; false on failure
     * Desc: If there is an entry for the given id, its associated
     *       priority is changed to new_priority and the data
     *       structure is modified accordingly.
     *       Otherwise, it is a failure (id not in pq or out of range)
     * Runtime:  O(log n)
     *
     */
    bool change_priority(int id, double new_priority) {
        if (id >= cap || id < 0 || ids.at(id) == 0) {
            return false;
        }
        int spot = ids.at(id);

        heap.at(spot)->priority = new_priority;

        if (min) {
            if (spot != 1 && heap.at(spot)->priority < heap.at(spot / 2)->priority) {
                perc_up_min(spot, id);
            } else if ((spot * 2) + 1 < firstEmpty && (heap.at(spot)->priority > heap.at(spot * 2)->priority || heap.at(spot)->priority > heap.at((spot * 2) + 1)->priority)) {
                perc_down_min(spot, id);
            } else if (spot * 2 == (firstEmpty - 1) && heap.at(spot)->priority > heap.at(spot * 2)->priority) {
                Node *temp = heap.at(spot * 2);

                int oldID = heap.at(spot * 2)->id;
                ids.at(oldID) = spot;
                ids.at(id) = spot * 2;

                heap.at(spot * 2) = heap.at(spot);
                heap.at(spot) = temp;
            }
        } else {  // max
            if (spot != 1 && heap.at(spot)->priority > heap.at(spot / 2)->priority) {
                perc_up_max(spot, id);
            } else if ((spot * 2) + 1 < firstEmpty && (heap.at(spot)->priority < heap.at(spot * 2)->priority || heap.at(spot)->priority < heap.at((spot * 2) + 1)->priority)) {
                perc_down_max(spot, id);
            } else if (spot * 2 == (firstEmpty - 1) && heap.at(spot)->priority < heap.at(spot * 2)->priority) {
                Node *temp = heap.at(spot * 2);

                int oldID = heap.at(spot * 2)->id;
                ids.at(oldID) = spot;
                ids.at(id) = spot * 2;

                heap.at(spot * 2) = heap.at(spot);
                heap.at(spot) = temp;
            }
        }
        return true;
    }

    void perc_down_min(int spot, int id) {
        int swap;
        Node *temp;
        int left;
        int right;

        while ((spot * 2) + 1 < firstEmpty && (heap.at(spot)->priority > heap.at(spot * 2)->priority || heap.at(spot)->priority > heap.at((spot * 2) + 1)->priority)) {
            left = spot * 2;
            right = left + 1;

            if (heap.at(left)->priority < heap.at(right)->priority) {
                swap = left;
            } else {
                swap = right;
            }

            int oldID = heap.at(swap)->id;
            ids.at(oldID) = spot;
            ids.at(id) = swap;

            temp = heap.at(swap);
            heap.at(swap) = heap.at(spot);
            heap.at(spot) = temp;
            spot = swap;
        }

        if ((spot * 2) == (firstEmpty - 1) && heap.at(spot)->priority > heap.at(spot * 2)->priority) {
            int oldID = heap.at(spot * 2)->id;
            ids.at(oldID) = spot;
            ids.at(id) = spot * 2;

            temp = heap.at(spot * 2);
            heap.at(spot * 2) = heap.at(spot);
            heap.at(spot) = temp;
            spot *= 2;
        }
        ids.at(id) = spot;
    }

    void perc_down_max(int spot, int id) {
        int swap;
        Node *temp;
        int left;
        int right;

        while ((spot * 2) + 1 < firstEmpty && (heap.at(spot)->priority < heap.at(spot * 2)->priority || heap.at(spot)->priority < heap.at((spot * 2) + 1)->priority)) {
            left = spot * 2;
            right = left + 1;

            if (heap.at(left)->priority > heap.at(right)->priority) {
                swap = left;
            } else {
                swap = right;
            }

            int oldID = heap.at(swap)->id;
            ids.at(oldID) = spot;
            ids.at(id) = swap;

            temp = heap.at(swap);
            heap.at(swap) = heap.at(spot);
            heap.at(spot) = temp;
            spot = swap;
        }

        if ((spot * 2) == (firstEmpty - 1) && heap.at(spot)->priority < heap.at(spot * 2)->priority) {
            int oldID = heap.at(spot * 2)->id;
            ids.at(oldID) = spot;
            ids.at(id) = spot * 2;

            temp = heap.at(spot * 2);
            heap.at(spot * 2) = heap.at(spot);
            heap.at(spot) = temp;
            spot *= 2;
        }
        ids.at(id) = spot;
    }

    /**
     * Function: remove_by_id
     * Parameters: element id
     * Returns: true on success; false on failure
     * Desc: if there is an entry associated with the given id, it is
     *       removed from the priority queue.
     *       Otherwise the data structure is unchanged and 0 is returned.
     * Runtime:  O(log n)
     *
     */
    bool remove_by_id(int id) {
        if (id >= cap || id < 0 || ids.at(id) == 0) {
            return false;
        } else if (ids.at(id) == (firstEmpty - 1)) {
            firstEmpty--;
            free(heap.at(ids.at(id)));
            heap.at(ids.at(id)) = nullptr;
            ids.at(id) = 0;
        } else {
            int newId = heap.at(firstEmpty - 1)->id;
            double newPriority = heap.at(firstEmpty - 1)->priority;
            int spot = ids.at(id);
            ids.at(id) = 0;
            ids.at(newId) = spot;
            free(heap.at(spot));
            heap.at(spot) = heap.at(firstEmpty - 1);
            heap.at(firstEmpty - 1) = nullptr;
            firstEmpty--;
            change_priority(newId, newPriority);
        }
        return true;
    }

    /**
     * Function: get_priority
     * Parameters: elment id
     *             reference double priority ("out" param)
     * Returns: true on success; false on failure
     * Desc: if there is an entry for given id, priority is assigned
     *       the associated priority and true is returned.
     *       Otherwise false is returned and priority has no meaning.
     * Runtime:  O(1)
     *
     */
    bool get_priority(int id, double &priority) {
        if (id >= cap || id < 0 || ids.at(id) == 0) {
            return false;
        }
        priority = heap.at(ids.at(id))->priority;
        return true;
    }

    /**
     * Function: delete_top
     * Parameters: reference int id and ref priority ("out" parameters)
     * Returns: true on success; false on failure (empty priority queue)
     * Desc: if queue is non-empty the "top" element is deleted and
     *       its id and priority are stored in *id and *priority;
     *       The "top" element will be either min or max (wrt priority)
     *       depending on how the priority queue was configured.
     *
     * Runtime:  O(log n)
     */
    bool delete_top(int &id, double &priority) {
        if (firstEmpty == 1) {
            return false;
        }
        id = heap.at(1)->id;
        priority = heap.at(1)->priority;
        remove_by_id(heap.at(1)->id);
        return true;
    }

    /**
     * Function: peek_top
     * Parameters: reference int id and ref priority ("out" parameters)
     * Returns: true on success; false on failure (empty priority queue)
     * Desc: if queue is non-empty information about the "top"
     *       element (id and priority) is stored in *id and *priority;
     *       The "top" element will be either min or max (wrt priority)
     *       depending on how the priority queue was configured.
     *
     * Runtime:  O(1)
     *
     */
    bool peek_top(int &id, double &priority) {
        if (firstEmpty == 1) {
            return false;
        }
        id = heap.at(1)->id;
        priority = heap.at(1)->priority;
        return true;
    }

    /**
     * Function:  capacity
     * Returns: capacity of priority queue (as set on creation)
     *
     * Runtime:   O(1)
     *
     */
    int capacity() {
        return cap;
    }

    /**
     * Function: size
     * Returns: number of elements currently in queue
     *
     * Runtime:  O(1)
     */
    int size() {
        return (firstEmpty - 1);
    }
};

#endif
