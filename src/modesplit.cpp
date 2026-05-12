#include "vhplatform.hpp"
#include "globaldefs.hpp"

#include "errs.hpp"
#include "misc/fileio.hpp"
#include "misc/membuff.hpp"

#include "c16cell.hpp"
#include "c256cell.hpp"
#include "sym8x8.hpp"

#include "gencells.hpp"
#include "genbmp.hpp"

#include "pgsym.hpp"


std::vector<uint8_t> SearchForSymmetry(const c16cell & c16);


//------------------------------------------------------------------------------
// Оптимизация псевдографики
//------------------------------------------------------------------------------
void ApplyPSGraph(c16cell & c16) {

    // Pseudo Graph : find center point
    sym8x8 cpnt;
    PGlyph psglyph;

    // Определение осей и параметров линий
    cpnt.set(c16.getsinglecell(CENTER_POINT_SYM_S) );
    psglyph.find_cpnt_single(cpnt);

    cpnt.set(c16.getsinglecell(CENTER_POINT_SYM_D) );
    psglyph.find_cpnt_double(cpnt);

    // Применение фильтра
    for( uint8_t i = SYM_PSEUDO_FIRST; i <= SYM_PSEUDO_LAST; i++) {
        const std::vector<uint8_t> & arr = psglyph.recreate(i);
        c16.applyxor(i, arr); }

}

//------------------------------------------------------------------------------
void applyfilter(c16cell & c16, std::vector<uint8_t> arr) {

    sym8x8 single;
    std::vector<uint8_t> arrcell;

    for(int i=0;i<256;i++) {
        arrcell = c16.getsinglecell(i);
        single.set(arrcell);
        single.applyfilter( (enattr) arr[i]);
        c16.setsym(i, single.get()); } }

//------------------------------------------------------------------------------
int optimizeproc(const c16cell & c16orig, char * argv[]) {

    // Parse argv : output params
    std::string fname_dst_att       = argv[3];

    // Parse argv : .bmp files
    std::string fname_orig_bmp      = argv[4];
    std::string fname_high_bmp      = argv[5];
    std::string fname_mirr_bmp      = argv[6];

    // Parse argv : .cells files
    std::string fname_orig_cells    = argv[7];
    std::string fname_high_cells    = argv[8];
    std::string fname_mirr_cells    = argv[9];

    // Выходное изображение
    c16cell c16xor;
    c16xor.set(c16orig);

    // В первую очередь - псевдографика
    ApplyPSGraph(c16xor);

    //
    std::vector<uint8_t> arrflts = SearchForSymmetry(c16orig);
    applyfilter(c16xor, arrflts);


    // ************
    // ***.cells***
    // ************

    CELLS256 c256orig, c256high, c256out;

    // Сгенерировать .cells оригинал
    c256original(c16orig, c256orig, fname_orig_cells);

    // Сгенерировать .cells (оритмизированный)
    c256optim(c16xor, c256out, arrflts, fname_mirr_cells);

    // Сгенерировать .cells оригинал + подсветка
    c256enlight(c16orig, c256high, c256out, arrflts, fname_high_cells);


    // ***************************
    // *** Сохранить аттрибуты ***
    // ***************************

    saveArrayToFile(arrflts, fname_dst_att);


    // ************
    // *** .bmp ***
    // ************

    // Сохраняем результат: original as b&w bmp
    genbmp(c256orig, fname_orig_bmp, true);

    // Сохраняем результат: original + highlight
    genbmp(c256high, fname_high_bmp, false);

    // Сохраняем результат: зеркальный b&w bmp
    genbmp(c256out, fname_mirr_bmp, true);

    return vok; }


//------------------------------------------------------------------------------
verr split_mode(int argc, char *argv[]) {

    if(argc != 10)
        return verror("SPLIT: Not enough parameters", err_split_not_enough_params);

    // Parse argv : input params
    std::string fname_src_orig_c16  = argv[2];

    c16cell c16;

    // Check Source File
    if(!fileExists(fname_src_orig_c16))
        return verror("Can't access source file " + fname_src_orig_c16, err_wrong_src_file);

    // Load source
    if (!c16.loadFromFile(fname_src_orig_c16))
        return verror("Cant load source file " + fname_src_orig_c16, err_load_src_issue);

    // Try to ptimize font
    verr tryopt = optimizeproc( c16, argv );
    if(tryopt) return tryopt;

    // --------------------
    // TODO: restore & save
    // --------------------

    // c16.restore();
    // if (!c16.saveToFile(fname_res_c16))
    //     return verror("Restore " + fname_res_c16, 3);

    return vok; }
