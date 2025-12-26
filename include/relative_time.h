#ifndef RELATIVE_TIME_H
#define RELATIVE_TIME_H

#include <stddef.h>

/**
 * Format elapsed time as a human-readable relative time string
 * 
 * Formats elapsed milliseconds into a compact string suitable for LED display:
 * - 0-89 seconds: "X s ago" (e.g., "5 s ago", "89 s ago")
 * - 90+ seconds: "X m ago" (e.g., "2 m ago", "15 m ago")
 * 
 * The value is always max 2 digits to fit on the display.
 * Values over 99 minutes are capped at "99 m ago".
 * 
 * :param unsigned long elapsedMs: Elapsed time in milliseconds
 * :param char* buffer: Output buffer for the formatted string
 * :param size_t bufferSize: Size of the output buffer (min 10 chars recommended)
 */
void formatRelativeTime(unsigned long elapsedMs, char* buffer, size_t bufferSize);

/**
 * Get the elapsed time value (without "ago" suffix)
 * Used primarily for testing
 * 
 * :param unsigned long elapsedMs: Elapsed time in milliseconds
 * :param int& value: Output value (the number)
 * :param char& unit: Output unit character ('s' or 'm')
 */
void getRelativeTimeComponents(unsigned long elapsedMs, int& value, char& unit);

#endif // RELATIVE_TIME_H
