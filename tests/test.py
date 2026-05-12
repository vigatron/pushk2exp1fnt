import unittest
import subprocess
import os
import shutil

from mods.examples import SRC_FILES

# Archivers relateds
from mods.archs import ARCHS_APPS, get_arr_filtered_version, get_arr_info
from mods.archs import ARCHS_APPS, ARCH_KKEYS
from mods.archs import multipack

from mods.paths import ROOT_DIR, BUILD_DIR, EXMPLS_DIR, RESULTS_DIR

# Markdown tables
from mods.mrkgen import gen_mrkd_tbl3, gen_mrkd_tbl4, gen_mrkd_arch

# files related
from mods.fsys import saveTextArray
from mods.fsys import CleanOrCreateFolder

# Filter related
from mods.fltmove import move_filter


# Список папок результатов
def PrepareResultFoldersList(flist):
    r = []
    for srcfld in flist:
        r.append(os.path.join(RESULTS_DIR, srcfld))
    return r


def PreCheckArchivers():
    arrarchs = get_arr_info()
    if(None == arrarchs):
        print("Issue with archivers, check availability");
        return False

    hdr = [ "Archiver", "Version", "Max Level", "Info" ]
    arr1 = ARCHS_APPS
    arr2 = get_arr_filtered_version(arrarchs)
    arr3 = ARCH_KKEYS
    arr4 = arrarchs
    content = gen_mrkd_tbl4(hdr, arr1, arr2, arr3, arr4)

    fname = "sysarchs.txt"
    fnamefull = os.path.join(RESULTS_DIR, fname)
    saveTextArray(content, fnamefull)

    return True


def convertfile(srcfile, dstfolder):

    TOOLNAME = "pushk2exp1fnt"
    TOOLBIN = os.path.join(BUILD_DIR, TOOLNAME)

    bname = os.path.basename(srcfile)
    fname_out_attr      = os.path.join(dstfolder, bname + "_att.bin")
    fname_out_orig_bmp  = os.path.join(dstfolder, bname + "_orig.bmp")
    fname_out_high_bmp  = os.path.join(dstfolder, bname + "_high.bmp")
    fname_out_mirr_bmp  = os.path.join(dstfolder, bname + "_mirr.bmp")
    fname_out_orig_256  = os.path.join(dstfolder, bname + "_orig.cells")
    fname_out_high_256  = os.path.join(dstfolder, bname + "_high.cells")
    fname_out_mirr_256  = os.path.join(dstfolder, bname + "_mirr.cells")

    arrcmd = [
        TOOLBIN,
        "s",
        srcfile,
        fname_out_attr,
        fname_out_orig_bmp,
        fname_out_high_bmp,
        fname_out_mirr_bmp,
        fname_out_orig_256,
        fname_out_high_256,
        fname_out_mirr_256 ]
    
    try:
        subprocess.run(arrcmd, check=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    except subprocess.CalledProcessError as e:
        print(f"Error while processing syscall: {e}")
        return None
    except FileNotFoundError:
        print( arrcmd )
        print(f"{TOOLNAME} utility not found. Make sure it is installed and in PATH.")
        return None

    return arrcmd


def STEP_PrepareResultsFolders(fldlist):
    print("***Creating results folders***")
    for srcfld in fldlist:
        CleanOrCreateFolder(srcfld)


def STEP_ConvertingExamples():
    r = []
    for idx in range(len(SRC_FILES)):
        srcfile = os.path.join(EXMPLS_DIR, SRC_FILES[idx])
        dstfolder = results_folders[idx]
        params = convertfile(srcfile, dstfolder)
        if None == params:
            return None
        r.append(params)
    return r


def STEP_MutlipackAll(rexamples):

    # pack original examples and splitted
    rtbl = []

    for item in rexamples:

        dstdir = os.path.dirname(item[7])

        # Call multipack on original
        orig_file = item[7]
        arrlog = multipack(orig_file, dstdir)
        if not arrlog:
            return None
        rtbl.append( arrlog )

        # Call multipack on split
        split_file = item[9]
        arrlog = multipack(split_file, dstdir)
        if not arrlog:
            return None
        rtbl.append( arrlog )

        # Filter settings for 'pushk2pre1move'
        fltname = os.path.join(ROOT_DIR, "cfg/cl_filter_b.yaml")

        # Transform layout for original file
        orig_b_file = move_filter(orig_file, fltname)

        # Call multipack on original(b)
        arrlog = multipack(orig_b_file, dstdir)
        if not arrlog:
            return None
        rtbl.append( arrlog )

        # Transform layout for split file
        split_b_file = move_filter(split_file, fltname)

        # Call multipack on split(b)
        arrlog = multipack(split_b_file, dstdir)
        if not arrlog:
            return None
        rtbl.append( arrlog )


    # ----------------------------------
    # Convert arrlog to markdown
    # ----------------------------------

    # Header text
    arrhdr = []
    markhdr = "| fname |"
    marksep = "|---|"
    for jcontent in rtbl[0]:
        markhdr += " " + jcontent['arch'] + " |"
        marksep += "---|"
    arrhdr.append("")
    arrhdr.append("<br>")
    arrhdr.append("")
    arrhdr.append(markhdr)
    arrhdr.append(marksep)

    # Generate Markdown for x8 examples
    arrmark = []
    sepcnt = 0
    for item in rtbl:

        # Header per file ( per 4 subtypes )
        if not (sepcnt % 4):
            arrmark.extend(arrhdr)
        sepcnt+=1

        line = "| " + item[0]['fname'] + " |"
        for jparam in item:
            val = jparam['fsize']
            valtxt = str(val) if type(val) is int else val
            line += " " + valtxt + " |"
        arrmark.append(line)

    # Write markdown benchmark table
    fname = "sysarchlog.txt"
    fnamefull = os.path.join(RESULTS_DIR, fname)
    print(fnamefull)
    saveTextArray(arrmark, fnamefull)

    return True


# ------------------------------------------------------------------------
if __name__ == "__main__":

    # Check system archivers before start
    if not PreCheckArchivers():
        exit(1)

    # ---
    results_folders = PrepareResultFoldersList(SRC_FILES)
    STEP_PrepareResultsFolders(results_folders)

    # ---
    rexamples = STEP_ConvertingExamples()
    if not rexamples:
        print("Conversion issue")
        exit(2)

    # ---
    rpack = STEP_MutlipackAll(rexamples)
    if not rpack:
        print("Multipack issue")
        exit(3)

    exit(0)
