/** Initializes the I/O device.
 *
 * This function has to be called once before any other function in this
 * file is executed. */
void init_io(void);

/** Indicates the progress of four independent processes.
 *
 * The returned value indicates the progress in the lower 4 bytes.
 * Each byte represents the status of one process' progress between 0
 * (at the beginning) and 255 (finished). */
unsigned int get_progress(void);

/** Prints an ASCII progress bar.
 *
 * This function prints a progress bar separated into 5 parts:
 *  - It is enclosed by a '[' and ']' on the left and right end respectively.
 *  - Inbetween those the done_symbol is printed from the left for up to
 *    the given percentage.
 *  - Then follows one active_symbol.
 *  - The remained till the closing bracket is filled with todo_symbol
 *    characters.
 *
 * Note: calling this function will first delete all previous output
 * on the current line. */
void progressbar(char done_symbol, char active_symbol, char todo_symbol, unsigned int percent);

/** Waits for any key press. */
void wait_key(void);

/** Reset all I/O devices to the initial values.
 *
 * This function shall be called after init_io and before the application exits. */
void shutdown_io(void);
