#ifndef PROGRAMME_MANGEMENT_H
#define PROGRAMME_MANGEMENT_H
#include <stdbool.h>

bool does_programme_exist(int programme_id);
bool add_programme(char* programme_name, int lecturer_id);
bool remove_programme(int programme_id);

#endif // PROGRAMME_MANGEMENT_H