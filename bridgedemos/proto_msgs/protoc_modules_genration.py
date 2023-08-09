key_word = ".proto"

protos = []
import os
import argparse
import shutil

_argparse = argparse.ArgumentParser()
_argparse.add_argument("protoc", type=str, help='''path to executable protoc, e.g., /user/local/bin/protoc or 'D:\\vcpkg\\installed\\x64-windows\\tools\\protobuf\\protoc.exe"''')
# _argparse.add_argument("libpath", type=str, help="path to protoc lib path, e.g., /user/local/lib")

if __name__ == '__main__':
    args = _argparse.parse_args()
    protoc = args.protoc
    #libpath = args.libpath
    #echo "export APOLLO_ROOT_DIR=$(pwd)" >> ~/.bashrc  && source ~/.bashrc
    for path, folders, files in os.walk("./bridge_msgs"):
        if len(folders) == 0:
            for file in files:
                if key_word in file:
                    print(path + "/*.proto")
                    protos.append(path + "/" + file)

    for proto in protos:
        #os.system(f"export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:{libpath} && {protoc} --cpp_out=./simcpp/ {proto}")
        os.system(f"{protoc} --cpp_out=./generated_cpp --python_out=./generated_py {proto}")
    shutil.copy("CMakeLists.txt", "./generated_cpp/bridge_msgs")
