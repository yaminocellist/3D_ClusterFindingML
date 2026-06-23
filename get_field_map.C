#include <ffamodules/CDBInterface.h>
#include <iostream>
#include <string>

void get_field_map() {
    std::string gap = CDBInterface::instance()->getUrl("FIELDMAP_GAP");
    std::string steel = CDBInterface::instance()->getUrl("FIELDMAP_STEEL");
    std::string tracking = CDBInterface::instance()->getUrl("FIELDMAP_TRACKING");
    std::cout << "FIELDMAP_GAP_PATH: " << gap << std::endl;
    std::cout << "FIELDMAP_STEEL_PATH: " << steel << std::endl;
    std::cout << "FIELDMAP_TRACKING_PATH: " << tracking << std::endl;
}
