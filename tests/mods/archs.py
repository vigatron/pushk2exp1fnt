import subprocess
import re
import os
import shutil

# ------------------------------
# Max Compression Rate
# ------------------------------
# arj a -m1 "$1".arj "$1"
# 7z -mx9 a "$1".7z "$1"
# rar a -m5 "$1".rar "$1"
# zip -9 "$1".zip "$1"
# bzip2 -k -9 "$1"
# gzip -k -9 "$1"
# xz -k -9 "$1"
# lzma -k -9 "$1"


ARCHS_APPS = [ "zip", "rar", "lzma", "7z", "xz", "zstd", "brotli"   , "bzip2"   , "gzip", "arj" ]

ARCHS_EXTS = [ "zip", "rar", "lzma", "7z", "xz", "zst" , "br"       , "bz2"     , "gz"  , "arj" ]

ARCH_KKEYS = [
    "-9",
    "-m5",
    "-9",
    "-mx9",
    "-9",
    "--ultra -22",
    "-q 11",
    "-9",
    "-9",
    "-jm"
]


ARCH_CMDS = {
    "zip":   ["zip", "--version"],
    "rar":   ["rar"],
    "lzma":  ["lzma", "--version"],
    "7z":    ["7z", "i"],
    "xz":    ["xz", "--version"],
    "zst":   ["zstd", "--version"],
    "br":    ["brotli", "--version"],
    "bz2":   ["bzip2", "--version"],
    "gz":    ["gzip", "--version"],
    "arj":   ["arj"]
}




# ----------------------------------------------------------------------------------------------------------
def packexts(fname : "str") -> list:
    exts = [f".{ext}" for ext in ARCHS_EXTS]
    return exts

# ----------------------------------------------------------------------------------------------------------
def packcmds(fname : str, packnames : list[str]) -> list:
    arr = [
        [ "zip"        , "-9"          , f"{fname}{packnames[0]}",          fname],
        [ "rar"        , "-m5"         , "a"   , f"{fname}{packnames[1]}",  fname],
        [ "lzma"       , "-9"          , "-k"  ,                            fname],
        [ "7z"         , "-mx9"        , "a"   , f"{fname}{packnames[3]}",  fname],
        [ "xz"         , "-9"          , "-k"  ,                            fname],
        [ "zstd"       , "--ultra"     , "-22" ,                            fname],
        [ "brotli"     , "-q"          , "11"  ,                            fname],
        [ "bzip2"      , "-9"          , "-k"  ,                            fname],
        [ "gzip"       , "-9"          , "-k"  ,                            fname],
        [ "arj"        , "-jm"         , "a"   , f"{fname}{packnames[9]}",  fname] ]
    return arr

# ----------------------------------------------------------------------------------------------------------
def get_info_arch(app: str) -> str:
    try:
        if app in ("rar", "7z"):
            # Эти утилиты печатают версию без аргументов
            result = subprocess.run(
                [app],
                stdout=subprocess.PIPE,
                stderr=subprocess.STDOUT,
                text=True,
                timeout=3
            )
            lines = result.stdout.strip().splitlines()
        else:
            # Общие флаги
            for flag in ["--version", "-v", "-V"]:
                result = subprocess.run(
                    [app, flag],
                    stdout=subprocess.PIPE,
                    stderr=subprocess.STDOUT,
                    text=True,
                    timeout=3
                )
                if result.stdout:
                    lines = result.stdout.strip().splitlines()
                    break
            else:
                return "unknown"

        # Ищем строку с номером версии
        for line in lines:
            if re.search(r"\d+\.\d+", line):
                return line.strip()
        # Если ничего не нашли — вернём первую строку
        return lines[0] if lines else "unknown"

    except FileNotFoundError:
        return None
    except Exception as e:
        return f"error: {e}"

# ----------------------------------------------------------------------------------------------------------
def get_arr_info():
    ret = []
    for app in ARCHS_APPS:
        ver = get_info_arch(app)
        if ver == None:
            print(app, " - Not found !")
            return None
        if "error" in ver:
            print(ver)
            return None 
        ret.append(ver)
    return ret

# ----------------------------------------------------------------------------------------------------------
def get_arr_filtered_version( arr: list[str]):
    filtered = []
    for line in arr:
        match = re.search(r"\d+(?:\.\d+)+", line)
        if match:
            prefix = line.split(":")[0]
            version = match.group(0)
            filtered.append(f"{version}")

    return filtered

# ----------------------------------------------------------------------------------------------------------
def multipack(fname, fldresults):

    r = []

    print("Source file    : " + fname)
    print("Results folder : " + fldresults)

    # copy src file to dst dir
    bname = os.path.basename(fname)
    # result_file = os.path.join(fldresults, bname)
    # shutil.copy2(fname, result_file)

    # Insert description : props of original source
    item = { 'arch' : "orig", 'fname' : bname, 'fsize' : os.path.getsize(fname) }
    r.append(item)

    # pack src file
    arrexts = packexts( os.path.basename(bname) )
    arrcmds = packcmds( os.path.basename(bname) , arrexts )

    for idx in range(len(arrcmds)):
        arrcmd = arrcmds[idx]
        print( arrcmd )

        try:
            subprocess.run(arrcmd, check=True, cwd=fldresults, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        except subprocess.CalledProcessError as e:
            print(f"Error while creating archive: {e}")
            return None
        except FileNotFoundError:
            print( arrcmd )
            print("utility not found. Make sure it is installed and in PATH.")
            return None
        
        zname = bname + "." + ARCHS_EXTS[idx]
        rnamefull = os.path.join(fldresults, zname)
        zsize = os.path.getsize(rnamefull)
        item = { 'arch' : arrcmd[0], 'fname' : zname, 'fsize' : zsize }
        r.append(item)

    return r


# ----------------------------------------------------------------------------------------------------------
if __name__ == "__main__":
    arrinfo = get_arr_info()
    if None == arrinfo:
        exit(1)
    for vermsg in arrinfo:
       print(f"{vermsg}")

