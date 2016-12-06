/**
 * Authors : Michał Kuźba, Aleksander Zendel
 */

#ifndef STRDEQUE_H
#define STRDEQUE_H

#ifdef __cplusplus
#include <cstddef>
#else
#include <stddef.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Create empty deque of strings and return its id.
 */
unsigned long strdeque_new();

/**
 * Delete a deque with a given id if it exists.
 */
void strdeque_delete(unsigned long id);

/**
 * Return the size of the deque with a given id if it exists.
 */
size_t strdeque_size(unsigned long id);

/**
 * Insert value to the deque with a given id before the position pos. If value is NULL
 * or deque does not exist it does nothing. If pos is larger than deque size it insert
 * the value to the end of the deque.
 */
extern void strdeque_insert_at(unsigned long id, size_t pos, const char* value);

/**
 * Remove the element on the position pos if deque with a given id exists and position is valid.
 */
void strdeque_remove_at(unsigned long id, size_t pos);

/**
 * Return pointer to an element from the deque with a given id from the pos position.
 * If deque does not exist or the position is not valid return NULL.
 */
const char* strdeque_get_at(unsigned long id, size_t pos);

/**
 * Delete all elements of the deque with a given id. If id is not valid it does nothing.
 */
void strdeque_clear(unsigned long id);

/**
 * Lexicographical comparison of two dequeus.
 * If id is not valid deque is treated as an empty deque.
 * Return:
 * -1 when deque(id1) < deque(id2)
 *  0 when deque(id1) == deque(id2)
 *  1 when deque(id1) > deque(id2)
 */
int strdeque_comp(unsigned long id1, unsigned long id2);

#ifdef __cplusplus
}
#endif

#endif /* STRDEQUE_H */
