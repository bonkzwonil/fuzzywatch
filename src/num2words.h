#pragma once

#include "string.h"

int fuzzy_time_to_words(int hours, int minutes, char* words, size_t length);
int date_to_words(int day, int month, int year, char *words, size_t length);

