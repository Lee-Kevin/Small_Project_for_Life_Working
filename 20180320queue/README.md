# queue

A thread safe type-agnostic header-only macro-based structure queue
implementation in C.

## Usage

Include the queue.h header file in your source. To create a queueable
structure, include the `queue_handle qh` member in the struct definition.

Before any other operations, ensure that QUEUE_INIT has been called, it is an
error to not do so. This initialises the queue and sets up the mutex and
conditions variables. Setting the queue to NULL will protect against errors
if a queue has not been

Freeing the queue does not free every element in the queue if they have been
dynamically allocated. This has to be done by popping all the elements in the
queue and freeing them manually.

Pushing the same element (at the same address) into the queue is not supported
and is an error. This is because the `next` member of the queue's `queue_handle`
will be overwritten. This will cause undefined behavior when pushing or popping
to or from the queue (see commented out test case).

```c
#include <stdlib.h>
#include <stdio.h>
#include "queue.h"

struct msg {
    char *content;
    queue_handle qh;
};

int main(void) {
    struct msg *msgs; // message queue
    struct msg m1, *m2;

    QUEUE_INIT(struct msg, msgs);

    m1.content = "abc";
    QUEUE_PUSH(msgs, &m1);

    printf("msgs size: %d\n", QUEUE_SIZE(msgs)); // msgs size: 1

    QUEUE_POP(msgs, m2);
    printf("m2 content: %s\n", m2->content); // m2 content: abc

    QUEUE_FREE(msgs);

    return EXIT_SUCCESS;
}
```

## Testing and building examples

To run tests run

```bash
make check
```

To compile the examples run

```bash
make examples
```

Both of the above can be done together by running

```bash
make
```
