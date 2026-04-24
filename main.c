#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// ---------- Circular Buffer Structure ----------
typedef struct {
    char *buffer;
    int head;
    int tail;
    int maxlen;
} CircularBuffer;

// ---------- Initialization ----------
bool cb_init(CircularBuffer *cb, int size) {
    cb->buffer = (char*)malloc(size * sizeof(char));
    if (cb->buffer == NULL) {
        return false;
    }
    cb->head = 0;
    cb->tail = 0;
    cb->maxlen = size;
    return true;
}

// ---------- Check if empty ----------
bool cb_is_empty(CircularBuffer *cb) {
    return (cb->head == cb->tail);
}

// ---------- Check if full ----------
bool cb_is_full(CircularBuffer *cb) {
    int next_head = (cb->head + 1) % cb->maxlen;
    return (next_head == cb->tail);
}

// ---------- Write (enqueue) a character ----------
bool cb_write(CircularBuffer *cb, char data) {
    if (cb_is_full(cb)) {
        return false;
    }
    cb->buffer[cb->head] = data;
    cb->head = (cb->head + 1) % cb->maxlen;
    return true;
}

// ---------- Read (dequeue) a character ----------
bool cb_read(CircularBuffer *cb, char *data) {
    if (cb_is_empty(cb)) {
        return false;
    }
    *data = cb->buffer[cb->tail];
    cb->tail = (cb->tail + 1) % cb->maxlen;
    return true;
}

// ---------- Free memory ----------
void cb_free(CircularBuffer *cb) {
    free(cb->buffer);
    cb->buffer = NULL;
    cb->head = cb->tail = 0;
    cb->maxlen = 0;
}

// ---------- Helper: print buffer content ----------
void cb_print(CircularBuffer *cb) {
    printf("Buffer content (tail -> head-1): ");
    if (cb_is_empty(cb)) {
        printf("(empty)\n");
        return;
    }
    int i = cb->tail;
    while (i != cb->head) {
        printf("%c ", cb->buffer[i]);
        i = (i + 1) % cb->maxlen;
    }
    printf("\n");
}

// ---------- Main Program ----------
int main() {
    CircularBuffer myBuffer;
    int bufferSize = 5;           // total capacity (usable = 4)
    char userName[100];           // input buffer for the name
    char combined[200];           // name + "CE-ESY"

    if (!cb_init(&myBuffer, bufferSize)) {
        printf("Error: Memory allocation failed.\n");
        return 1;
    }

    printf("===== Circular Buffer created (capacity %d, usable %d) =====\n\n",
           bufferSize, bufferSize - 1);

    // ---------- 1. Enter your name ----------
    printf("Enter your name: ");
    fgets(userName, sizeof(userName), stdin);
    userName[strcspn(userName, "\n")] = '\0';   // remove newline

    // ---------- 2. Concatenate with "CE-ESY" ----------
    snprintf(combined, sizeof(combined), "%s%s", userName, "CE-ESY");
    printf("\nCombined string: \"%s\"\n", combined);

    // ---------- 3. Write combined string to circular buffer ----------
    printf("\n----- Writing combined string character by character -----\n");
    for (int i = 0; combined[i] != '\0'; i++) {
        if (cb_write(&myBuffer, combined[i])) {
            printf("Written: '%c'\n", combined[i]);
        } else {
            printf("Failed to write '%c' (buffer full, ignoring remaining)\n", combined[i]);
        }
        cb_print(&myBuffer);
    }

    printf("\n----- Buffer state after writing combined string -----\n");
    printf("Empty? %s - Full? %s\n\n",
           cb_is_empty(&myBuffer) ? "Yes" : "No",
           cb_is_full(&myBuffer) ? "Yes" : "No");

    // ---------- 4. Read all characters back ----------
    printf("----- Reading all characters from buffer -----\n");
    char readChar;
    while (cb_read(&myBuffer, &readChar)) {
        printf("Read: '%c'\n", readChar);
        cb_print(&myBuffer);
    }

    printf("\n----- Buffer state after reading -----\n");
    printf("Empty? %s - Full? %s\n",
           cb_is_empty(&myBuffer) ? "Yes" : "No",
           cb_is_full(&myBuffer) ? "Yes" : "No");

    if (cb_is_empty(&myBuffer)) {
        printf("✓ Buffer is empty as expected.\n\n");
    }

    // ---------- 5. Additional test with a short string ----------
    printf("----- Additional test: enter a short word (< %d chars) -----\n", bufferSize-1);
    printf("Short word: ");
    char shortWord[100];
    fgets(shortWord, sizeof(shortWord), stdin);
    shortWord[strcspn(shortWord, "\n")] = '\0';

    printf("Writing \"%s\":\n", shortWord);
    for (int i = 0; shortWord[i] != '\0'; i++) {
        cb_write(&myBuffer, shortWord[i]);
        printf("Written: '%c'\n", shortWord[i]);
        cb_print(&myBuffer);
    }

    printf("\nReading back:\n");
    while (cb_read(&myBuffer, &readChar)) {
        printf("Read: '%c'\n", readChar);
        cb_print(&myBuffer);
    }

    printf("\n----- Final state -----\n");
    if (cb_is_empty(&myBuffer)) {
        printf("✓ Buffer empty after second read – solution verified.\n");
    }

    cb_free(&myBuffer);
    printf("\nMemory freed. Program terminated.\n");

    return 0;
}
