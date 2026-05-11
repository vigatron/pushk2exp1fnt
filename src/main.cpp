#include "vhplatform.hpp"

#include "errs.hpp"
#include "misc/fileio.hpp"
#include "misc/membuff.hpp"

#include "bmpbw.hpp"
#include "c16cell.hpp"
#include "c256cell.hpp"
#include "sym8x8.hpp"

#include "gencells.hpp"
#include "genbmp.hpp"


// TODO: Check CP866 font :
//  syms match 79 O, 111 o, 142 O, 174 o
//  syms match 148 Ф, 228 'ф'


verr split_mode(int argc, char *argv[]);
verr merge_mode(int argc, char *argv[]);

//------------------------------------------------------------------------------
int main(int argc, char *argv[]) {

    // No params given
    if(argc < 2)
        return verror("No input parameters", err_no_args_params);

    // Check mode
    std::string cmdstr = argv[1];
    if(cmdstr == "v") {
        printf("Version 1.0\n");
        return vok; }

    // Split mode
    if(cmdstr == "s") {
        return split_mode(argc, argv); }

    // Merge mode
    if(cmdstr == "m") {
        return merge_mode(argc, argv); }

    // Unknown command
    return verror("Command unsupported " + cmdstr, err_unsupported); }