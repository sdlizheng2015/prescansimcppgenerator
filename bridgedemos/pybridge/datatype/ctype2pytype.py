"""
DATE: 2022/5/8;16:15
PROJECT: install.bat/CType2PyType.py
AUTHOR: OldYoung
E-mail: yyangjlu@163.com
Bugs are everywhere!!!

Example.h:

1. Every line of annotation should always begin with '*', '/*', '//':

!!!Right!!!:
/*******************************************************
 * @file
 * ICD of the Runtime Data Bus (RDB)
 ********************************************************/

# This is an API function

!!!Wrong!!!:
/*
    This is an API function
*/

2. Marco definition:

#define RDB_PKG_ID_START_OF_FRAME               1     /**< sent as first package of a simulation frame
#define RDB_PKG_ID_END_OF_FRAME                 2     /**< sent as last package of a simulation frame
#define RDB_PKG_ID_COORD_SYSTEM                 3     /**< defines a (custom) co-ordinate system    

3. Struct definition, like below

typedef struct
{
    uint32_t id;                                    /**< unique object ID
    uint8_t  category;                              /**< object category
    uint8_t  type;                                  /**< object type
    int16_t  modelId;                               /**< visual model ID
    char     name[RDB_SIZE_OBJECT_NAME];            /**< symbolic name
    char     modelName[RDB_SIZE_OBJECT_NAME];       /**< model name associated to an object
    char     fileName[RDB_SIZE_FILENAME];           /**< filename associated to an object
    uint16_t flags;                                 /**< object configuration flags
    uint16_t spare0;                                /**< reserved for future use
    uint32_t spare1;                                /**< reserved for future use
} RDB_OBJECT_CFG_t;

...
"""

UnitSpace = " "
right = "# To convert CPP data define header to CPython type\n" \
        "# 1. This file is generated automatically;\n" \
        "# 2. All rights reserved by Yang;\n" \
        "# 3. Now only Struct and #define constant are supported;\n" \
        "# 4. Any questions email at yyangjlu@163.com.\n"

map_cpp_to_py = {
    "int8_t": "c_int8",
    "uint8_t": "c_uint8",
    "int16_t": "c_int16",
    "uint16_t": "c_uint16",
    "int32_t": "c_int32",
    "uint32_t": "c_uint32",
    "int64_t": "c_int64",
    "uint64_t": "c_uint64",
    "float": "c_float",
    "double": "c_double",
    "char": "c_char",
    "uint": "c_uint32",
    "bool": "c_uint8",
}


def right_py_clarification(file):
    file.write("# " + "#" * 70 + "\n")
    file.write(right)
    file.write("# " + "#" * 70 + "\n" * 2)


def convert_cpp_to_cpy(cpp_path: str, cpy_path: str):
    """
    :param cpp_path: cpp/c type
    :param cpy_path: py c_type
    :return:
    """
    with open(cpp_path, "rt", newline="\n", encoding="utf-8") as cpp_h, \
            open(cpy_path, "wt", newline="\n", encoding="utf-8") as cpy_h:
        right_py_clarification(cpy_h)
        cpy_h.write(f"from datatype.base import PackedStructure\n")
        cpy_h.write(f"from ctypes import *\n")
        cpy_h.write("\n" * 2)

        while True:
            line = cpp_h.readline().strip("\n").strip()
            if line.startswith("#define"):
                define_info = line.split()
                if len(define_info) >= 3:
                    comment = "no comments"
                    if len(define_info) >= 4:
                        comment = " ".join(define_info[3:])
                    cpy_h.write(f"{define_info[1]} = {define_info[2]}  # {comment}\n")
                else:
                    continue
            elif line.startswith("typedef struct") or line.startswith("typedef union"):
                """
                typedef struct/union
                {
                    uint16_t  magicNo;      /**< must be RDB_MAGIC_NO (35712)*/                   
                    uint16_t  version;      /**< upper byte = major, lower byte = minor*/  
                    uint32_t  headerSize;   /**< size of this header structure when transmitted
                                                 here end. */         
                    uint32_t  dataSize;     /**< size of data following the header*/  
                    uint32_t  frameNo;      /**< number of the simulation frame*/      
                    double    simTime;      /**< simulation time*/             
                } RDB_MSG_HDR_t;
                """

                cpy_h.write("\n")
                st_name = ""
                st_varNames = []
                st_varTypes = []
                st_comments = []
                st_type = "PackedStructure" if line.startswith("typedef struct") else "Union"

                in_struct = True
                while in_struct:
                    struct_line = cpp_h.readline().strip("\n").strip()

                    if struct_line.startswith("}"):  # End of Struct define
                        st_info = struct_line.split()
                        st_name = st_info[1]
                        if ";" in st_name:
                            st_name = st_name.replace(";", "")

                        cpy_h.write(f"class {st_name}({st_type}):\n")
                        cpy_h.write(4 * UnitSpace + f"_fields_ = [\n")
                        for index, i in enumerate(st_varNames):
                            cpy_h.write(8 * UnitSpace + f"(\"{i}\", {st_varTypes[index]})," +
                                        2 * UnitSpace + st_comments[index] + "\n")
                        cpy_h.write(4 * UnitSpace + f"]\n")
                        cpy_h.write("\n")

                        in_struct = False
                        continue
                    else:
                        if struct_line.startswith("{"):  # by default format
                            continue

                        u"""
                        这里很不太严格，要求struct注释里面不能出现’;‘
                        """
                        if ";" not in struct_line:  # Here is not strict
                            continue

                        st_info = struct_line.split()

                        possible_array_var = st_info[1]

                        if ";" in possible_array_var:
                            possible_array_var = possible_array_var.replace(";", "")

                        if "[" in possible_array_var:
                            name_num = possible_array_var.split("[")
                            var_name = name_num[0]
                            array_num = name_num[1].split("]")[0] + "*"  # shmtypes array format: num*c_types
                        else:
                            var_name = possible_array_var
                            array_num = ""

                        st_varNames.append(var_name)
                        if len(st_info) < 3:
                            comment = "no comments"
                        else:
                            comment = " ".join(st_info[2:])
                        st_comments.append("# " + comment)

                        base_type = st_info[0]
                        if base_type in map_cpp_to_py.keys():
                            ctype = map_cpp_to_py[base_type]
                        else:
                            ctype = base_type
                        st_varTypes.append(array_num + ctype)

            elif line.startswith("#pragma pack(pop)"):
                break
            else:
                continue


if __name__ == '__main__':
    convert_cpp_to_cpy("./cpp_h/RDB.h", "./RDB_MSG.py")
