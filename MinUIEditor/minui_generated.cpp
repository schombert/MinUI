#include "../common_files/minui_interfaces.hpp"

namespace min_ui {

// implementations of:
uint32_t defined_datatype();
uint32_t datatype_size(uint32_t data_type_id);
void run_datatype_constructor(char* address, uint32_t data_type_id);
void run_datatype_destructor(char* address, uint32_t data_type_id); uint32_t defined_datatype();


}

#include "../common_files/minui_core_impl.cpp"
