import os
import shutil



def saveTextArray(lines : list[str], fname : str):
    with open(fname, "w", encoding="utf-8") as file:
        file.write("\n".join(lines))


def CleanOrCreateFolder(fldname : str):
    print("Preparing folder: " + fldname)
    if os.path.exists(fldname):
        # удалить все содержимое папки
        shutil.rmtree(fldname)
        # заново создать пустую папку
        os.makedirs(fldname)
    else:
        # если папки нет — создать
        os.makedirs(fldname)
