#include <stdbool.h>
#include <data management system/datamanager.h>

bool does_programme_exist(int programme_id) {
    return is_programme_exist(programme_id);
}

bool add_programme(char* programme_name, int lecturer_id) {
    return create_programme_record(programme_name, lecturer_id);
}
bool remove_programme(int programme_id) {
    return delete_programme_record(programme_id);
}