import os
import subprocess

def move_filter(srcname, fltname):
    folder = os.path.dirname(srcname)
    fname_orig = os.path.basename(srcname)
    fname_orig_noext = os.path.splitext(fname_orig)[0]
    fname_move = fname_orig_noext + "_b.cells"
    dstname = os.path.join(folder, fname_move)
    
    TOOLNAME = "pushk2pre1move"

    arrcmd = [
        TOOLNAME,
        srcname,
        "0",
        fltname,
        dstname ]

    try:
        subprocess.run(arrcmd, check=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    except subprocess.CalledProcessError as e:
        print(f"Error while processing syscall: {e}")
        return None
    except FileNotFoundError:
        print( arrcmd )
        print(f"{TOOLNAME} utility not found. Make sure it is installed and in PATH.")
        return None

    return dstname
