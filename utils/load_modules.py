import os
import importlib
from typing import List, Tuple


def get_cls(module_names: List[str], pkg: str, sub_cls_name: str = "") \
        -> Tuple[List[callable], List[callable]]:
    """
    TODO: to get class and subclass in a module with '__load__module__'
    Example:
    root:
    --package1:
    ----module1.py
          '''
          __load__module__ = ["module1"]

          class module1:
              def __init__(self):
                  ...

              class sub_module:
                  def __init__(self):
                      ...
          '''
    ----module2.py
          '''
          __load__module__ = ["module2"]

          class module2:
              def __init__(self):
                  ...

              class sub_module:
                  def __init__(self):
                      ...
          '''
    :param module_names: e.g., [.module1, module2]
    :param pkg: e.g., root.package1
    :param sub_cls_name: subclass defined inside 'module.__load__module__' class, e.g., "sub_module"
    :return:
    """
    module_classes = []
    module_sub_classes = []
    for module_name in module_names:
        module = importlib.import_module(".{}".format(module_name), package=pkg)
        for cls in module.__load_module__:
            module_classes.append(getattr(module, cls))
    if sub_cls_name:
        for module_class in module_classes:
            module_sub_classes.append(getattr(module_class, sub_cls_name))
    return module_classes, module_sub_classes
