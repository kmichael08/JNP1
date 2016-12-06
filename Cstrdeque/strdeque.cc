#include <iostream>
#include <deque>
#include <map>
#include <vector>
#include <cassert>
#include <climits>

#include "strdeque.h"

#ifdef NDEBUG
const bool DEBUG = false;
#else
const bool DEBUG = true;
#endif 

typedef std::map<unsigned long, std::deque<std::string> > str_deque;

static unsigned long globalId = 1;

void init() {
    static std::ios_base::Init init;
}

void log_debug(std::string message) {
    init();
    if (DEBUG) std::cerr << message << std::endl;
}

/**
 * Prevention from static initialization order fiasco.
 */
str_deque& queues(){
    static str_deque* queues = new str_deque;
    return *queues;
}

bool does_queue_exist(unsigned long id, std::string method_name) {
    if (!queues().count(id)) {
        log_debug(method_name + " : Queue with id " + std::to_string(globalId) + " doesn't exist");
        return false;
    }
    return true;
}

unsigned long strdeque_new() {
    globalId++;

    // id larger than MAX long
    assert (globalId < ULONG_MAX);
    log_debug("strdeque_new: Queue created with id " + std::to_string(globalId));

    queues()[globalId] = std::deque<std::string>();
    return globalId;
}

void strdeque_delete(unsigned long id) {
    if (does_queue_exist(id,"strdeque_delete")) {
        queues().erase(id);
        log_debug("strdeque_delete: Queue with id " + std::to_string(globalId) + " deleted");
    }
}

size_t strdeque_size(unsigned long id) {
    if (does_queue_exist(id,"strdeque_size"))
        return queues()[id].size();
    return 0;
}

void strdeque_insert_at(unsigned long id, size_t pos, const char* value) {
    if (does_queue_exist(id,"strdeque_insert_at") && value != NULL) {

        std::deque<std::string>::iterator insertPos = queues()[id].begin();
        if (pos <= strdeque_size(id))
            insertPos += pos;
        else
            insertPos = queues()[id].end();

        queues()[id].insert(insertPos, std::string(value));
    } else {
        if (value==NULL) log_debug("strdeque_insert_at: null value");
    }
}

void strdeque_remove_at(unsigned long id, size_t pos) {
    if (does_queue_exist(id,"strdeque_remove_at") && pos < strdeque_size(id))
        queues()[id].erase(queues()[id].begin() + pos);
    else {
        if (!(pos < strdeque_size(id))) log_debug("strdeque_remove_at:incorrect position");
    }

}

const char* strdeque_get_at(unsigned long id, size_t pos) {
    if (does_queue_exist(id, "strdeque_get_at") && pos < strdeque_size(id)) {
        std::string res = queues()[id][pos];
        char *result = new char[res.size() + 1];
        std::copy(res.begin(), res.end(), result);
        result[res.size()] = '\0'; // terminating 0
        return result;
    }
    else {
        if (!(pos < strdeque_size(id))) log_debug("strdeque_remove_at:incorrect position");
        return NULL;
    }
}

/**
 * Clear the content of the queue with a given id.
 */
void strdeque_clear(unsigned long id) {
    if (does_queue_exist(id,"strdeque_clear"))
        queues()[id].clear();
}

/**
 * Compares two queues lexicographically.
 */
int strdeque_comp(unsigned long id1, unsigned long id2) {
    does_queue_exist(id1,"strdeque_comp");
    does_queue_exist(id2,"strdeque_comp");
    std::deque<std::string>::iterator it1 = queues()[id1].begin(), it2 = queues()[id2].begin();

    /*
     * If both are empty then return 0. If only the second is empty return 1.
     * If only the first is empty return -1.
     */
    if (queues()[id1].empty() || queues()[id2].empty())
        return (queues()[id2].empty() - queues()[id1].empty());

    while (it1 != queues()[id1].end() && it2 != queues()[id2].end()) {
        if (*it1 < *it2)
            return -1;
        if (*it1 > *it2)
            return 1;

        it1++;
        it2++;
    }

    if (it1 != queues()[id1].end())
        return -1;
    if(it2 != queues()[id2].end())
        return 1;

    return 0;
}

