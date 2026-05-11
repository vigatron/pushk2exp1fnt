#pragma once

#include "vhplatform.hpp"
#include "c16cell.hpp"
#include "c256cell.hpp"


verr c256original(
    const c16cell & c16orig,
    CELLS256 & c256,
    const std::string fname_orig_cells);

verr c256enlight(
    const c16cell & c16orig,
    CELLS256 & c256enl,
    const CELLS256 & c256out,
    const std::vector<uint8_t> & arrflts,
    const std::string fname);

verr c256optim(
    const c16cell & c16xor,
    CELLS256 & c256,
    const std::vector<uint8_t> & arrflts,
    const std::string & fname_mirr_cells );
