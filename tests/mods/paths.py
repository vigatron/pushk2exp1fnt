import os

LOCAL_DIR = os.path.dirname(os.path.abspath(__file__))
ROOT_DIR = os.path.dirname(os.path.dirname(LOCAL_DIR))
BUILD_DIR = os.path.join(ROOT_DIR, "build")
EXMPLS_DIR = os.path.join(ROOT_DIR, "examples")
RESULTS_DIR = os.path.join(ROOT_DIR, "results")
